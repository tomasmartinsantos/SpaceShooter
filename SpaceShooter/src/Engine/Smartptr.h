#ifndef SMARTPTR_H
#define SMARTPTR_H

template<typename T> class WPtr;

class RC {
public:
    RC()                                : mCount(0), mWeakCount(0) {}
    void Inc()                          { mCount++; IncWeak(); }
    unsigned long Dec()                 { DecWeak(); return --mCount; }
    void IncWeak()                      { mWeakCount++; }
    unsigned long DecWeak()             { return --mWeakCount; }
    unsigned long GetCount()     const  { return mCount;}
    unsigned long GetWeakCount() const  { return mWeakCount; }
private:
    unsigned long mCount;
    unsigned long mWeakCount;
};

template<typename T>
class Ptr {
public:
    Ptr()                                   : mData(0), mRef(0) { mRef = new RC(); mRef->Inc(); }
    Ptr(T* p)                               : mData(p), mRef(0) { mRef = new RC(); mRef->Inc(); }
    Ptr(T* p, RC* r)                        : mData(p), mRef(r) { mRef->Inc(); }
    Ptr(const Ptr<T>& p)                    : mData(p.mData), mRef(p.mRef) { mRef->Inc(); }
    Ptr(const WPtr<T>& p);
    ~Ptr()                                  { if ( mRef->Dec() == 0 ) { delete mData; } if ( mRef->GetWeakCount() == 0 ) { delete mRef; } }
    T& operator*() const                    { return *mData; }
    T* operator->() const                   { return mData; }
    Ptr<T>& operator=(const Ptr<T>& p)      { if ( *this != p ) { if ( mRef->Dec() == 0 ) { delete mData; } if ( mRef->GetWeakCount() == 0 ) { delete mRef; } mData = p.mData; mRef = p.mRef; mRef->Inc(); } return *this; }
    bool operator==(const Ptr<T>& p) const  { return this->mData == p.mData; }
    bool operator!=(const Ptr<T>& p) const  { return this->mData != p.mData; }
    bool operator==(const T* const p) const { return this->mData == p; }
    bool operator!=(const T* const p) const { return this->mData != p; }
    
    template <typename Y> Ptr<Y> UpCast();
    template <typename Y> Ptr<Y> DownCast();
    Ptr<const T> ConstCast() const { return Ptr<const T>(mData, mRef); }
private:
    T* mData;
    RC* mRef;

    friend class WPtr<T>;
};

template<typename T>
class WPtr {
public:
    WPtr()                                      : mData(0), mRef(0) { mRef = new RC(); mRef->IncWeak(); }
    WPtr(T* p)                                  : mData(p), mRef(0) { mRef = new RC(); mRef->IncWeak(); }
    WPtr(T* p, RC* r)                           : mData(p), mRef(r) { mRef->IncWeak();}
    WPtr(const WPtr<T>& p)                      : mData(p.mData), mRef(p.mRef) { mRef->IncWeak(); }
    WPtr(const Ptr<T>& p)                       : mData(p.mData), mRef(p.mRef) { mRef->IncWeak(); }
    ~WPtr()                                     { if ( mRef->DecWeak() == 0 ) { delete mRef; } }
    T& operator*() const                        { return *mData; }
    T* operator->() const                       { return mData; }
    WPtr<T>& operator=(const WPtr<T>& p)        { if ( *this != p ) { if (mRef->DecWeak() == 0) { delete mRef; } mData = p.mData; mRef = p.mRef; mRef->IncWeak(); } return *this; }
    bool operator==(const WPtr<T>& p) const     { return this->mData == p.mData; }
    bool operator!=(const WPtr<T>& p) const     { return this->mData != p.mData; }
    bool operator==(const T* const p) const     { return this->mData == p; }
    bool operator!=(const T* const p) const     { return this->mData != p; }
    
    template <typename Y> WPtr<Y> UpCast();
    template <typename Y> WPtr<Y> DownCast();
    WPtr<const T> ConstCast() const { return WPtr<const T>(mData, mRef); }
private:
    T* mData;
    RC* mRef;

    friend class Ptr<T>;
};

//template <typename T>
//Ptr<T>::Ptr(const WPtr<T>& p) : mData(p.mData), mRef(p.mRef) {
//    if ( !mRef ) mRef = new RC();
//    mRef->Inc();
//}

template <typename T>
Ptr<T>::Ptr(const WPtr<T>& p) : mData(0), mRef(0)
{
    // In case the reference counter is > 0, then create the SmartPointer
    if (p.mRef && p.mRef->GetCount() > 0)
    {
        mData   = p.mData;
        mRef    = p.mRef;
        mRef->Inc();
    }
    // Otherwise, create an empty SmartPointer
    else
    {
        mRef = new RC(); mRef->Inc();
    }
}

template <typename T>
template <typename Y>
Ptr<Y> Ptr<T>::UpCast() {
    if ( static_cast<Y*>(mData) )
        return Ptr<Y>(mData, mRef);
    else
        return Ptr<Y>();
}

template <typename T>
template <typename Y>
Ptr<Y> Ptr<T>::DownCast() {
    if ( dynamic_cast<Y*>(mData) )
        return Ptr<Y>((Y*)mData, mRef);
    else
        return Ptr<Y>();
}

template <typename T>
template <typename Y>
WPtr<Y> WPtr<T>::UpCast() {
    if ( static_cast<Y*>(mData) )
        return WPtr<Y>(mData, mRef);
    else
        return WPtr<Y>();
}

template <typename T>
template <typename Y>
WPtr<Y> WPtr<T>::DownCast() {
    if ( dynamic_cast<Y*>(mData) )
        return WPtr<Y>((Y*)mData, mRef);
    else
        return WPtr<Y>();
}

#endif // SMARTPTR_H
