#ifndef UGINE_IMAGE_H
#define UGINE_IMAGE_H

#include "Types.h"
#include "String.h"


class Image
{
    // Methods
public:
    static Ptr<Image> Create(const String& filename, float FixImageRot = 0.0f, uint16 hframes = 1, uint16 vframes = 1);

    virtual bool IsValid() const { return mGltex != 0; }
    virtual const String& GetFilename() const { return mFilename; }
    virtual uint32 GetNumFrames() const { return mHframes * mVframes; }
    virtual uint16 GetHFrames() const { return mHframes; }
    virtual uint16 GetVFrames() const { return mVframes; }
    virtual uint16 GetWidth() const { return mWidth / mHframes; }
    virtual uint16 GetHeight() const { return mHeight / mVframes; }
    virtual void SetHandle(int32 handlex, int32 handley) { mHandlex = handlex; mHandley = handley; }
    virtual void SetMidHandle() { SetHandle(GetWidth()/2, GetHeight()/2); }
    virtual int32 GetHandleX() const { return mHandlex; }
    virtual int32 GetHandleY() const { return mHandley; }
    virtual void Bind() const;
    virtual double GetLastU() const { return mLastU; }
    virtual double GetLastV() const { return mLastV; }

    virtual float GetFixImageRotation() const { return mFixAngleRotation; }
protected:
    Image(const String& filename, float FixImageRot = 0.0f, uint16 hframes = 1, uint16 vframes = 1);
    virtual ~Image();

    virtual uint32 GetTexId() const { return mGltex; }

    // Variables
protected:
    WPtr<Image> mThis;
private:
    String      mFilename;
    float       mFixAngleRotation;
    uint16      mHframes, mVframes;
    uint16      mWidth, mHeight;
    int32       mHandlex, mHandley;
    uint32      mGltex;
    double      mLastU, mLastV;
    
    friend class Ptr<Image>;
    friend class Ptr<const Image>;
};

#endif
