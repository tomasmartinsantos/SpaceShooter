#ifndef WLIFEBAR_H
#define WLIFEBAR_H

#include "../../Engine/Types.h"
#include "Widget.h"

class WLifebar : public Widget
{
    // Methods
public:
    static Ptr<WLifebar> Create();
    static Ptr<WLifebar> Create(const int ID, const Transform2D& Tranform2D);

    virtual void Init();
    virtual void Update(float elapsed);
    virtual void Render();

    int         GetMaxLife() const { return mMaxLife; }
    void        SetMaxLife(int Life) { mMaxLife = Life; }
    int         GetCurrentLife() const { return mCurrentLife; }
    void        SetCurrentLife(int Life) { mCurrentLife = Life; }

    float       GetWidth() const { return mWidth; }
    void        SetWidth(float Value) { mWidth = Value; }
    float       GetHeight() const { return mHeight; }
    void        SetHeight(float Value) { mHeight = Value; }

    void        SetColor(uint8 r, uint8 g, uint8 b, uint8 a) { Red = r; Green = g; Blue = b; Alpha = a; }

protected:
    WLifebar();
    WLifebar(const int ID, const Transform2D& Tranform2D);
    virtual ~WLifebar();

    // Variables
private:
    float       mWidth;
    float       mHeight;
    float       mRenderHeight;

    uint8 Red, Green, Blue, Alpha;

    int         mCurrentLife;
    int         mMaxLife;

    friend class Ptr<WLifebar>;
    friend class Ptr<const WLifebar>;
};

#endif