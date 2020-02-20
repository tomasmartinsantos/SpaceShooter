#include "../../Engine/Audio/Audiosource.h"
#include "../../Engine/Renderer.h"
#include "../Components/CAnimation.h"
#include "WLifebar.h"


Ptr<WLifebar> WLifebar::Create()
{
    Ptr<WLifebar> p = new WLifebar();
    p->mThis = p.UpCast<Widget>();
    return p;
}

Ptr<WLifebar> WLifebar::Create(const int ID, const Transform2D& Tranform2D)
{
    Ptr<WLifebar> p = new WLifebar(ID, Tranform2D);
    p->mThis = p.UpCast<Widget>();
    return p;
}

WLifebar::WLifebar()
{
    mID             = -1;
    mParentWidget   = nullptr;
    mThis           = nullptr;
    mAnimationComp  = nullptr;
    SetTransform(Transform2D());
    mWidth          = 0.0f;
    mHeight         = 0.0f;
    Red             = 0;
    Green           = 0;
    Blue            = 0;
    Alpha           = 0;
    mCurrentLife    = 0;
    mMaxLife        = 0;
}

WLifebar::WLifebar(const int ID, const Transform2D& Tranform2D)
{
    mID             = ID;
    mParentWidget   = nullptr;
    mThis           = nullptr;
    mAnimationComp  = nullptr;
    SetTransform(Tranform2D);
    mWidth          = 0.0f;
    mHeight         = 0.0f;
    Red             = 0;
    Green           = 0;
    Blue            = 0;
    Alpha           = 0;
    mCurrentLife    = 0;
    mMaxLife        = 0;
}

WLifebar::~WLifebar()
{
    Widget::~Widget();
}

void WLifebar::Init()
{
    Widget::Init();
    mRenderHeight = mHeight;
}

void WLifebar::Update(float elapsed)
{
    Widget::Update(elapsed);
    mRenderHeight = (mHeight * mCurrentLife) / mMaxLife;
}

void WLifebar::Render()
{
    Widget::Render();
    Renderer::Instance()->SetColor(Red, Green, Blue, Alpha);
    Renderer::Instance()->SetBlendMode(Renderer::ALPHA);
    Renderer::Instance()->DrawRect(GetTransform().Position.x, GetTransform().Position.y, mWidth*GetTransform().Scale.x, mRenderHeight*GetTransform().Scale.y);
}