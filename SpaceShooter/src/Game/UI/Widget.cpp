#include "../../Engine/Audio/Audiosource.h"
#include "../Components/CAnimation.h"
#include "../../Engine/Renderer.h"
#include "../../Engine/Image.h"
#include "../../Engine/Math.h"
#include "Widget.h"


Ptr<Widget> Widget::Create()
{
    Ptr<Widget> p = new Widget();
    p->mThis = p;
    return p;
}

Ptr<Widget> Widget::Create(const int ID, const Transform2D& Tranform2D)
{
    Ptr<Widget> p = new Widget(ID, Tranform2D);
    p->mThis = p;
    return p;
}

Widget::Widget()
{
    mID             = -1;
    mParentWidget   = nullptr;
    mThis           = nullptr;
    mTransform      = Transform2D();
    mAnimationComp  = nullptr;
}

Widget::Widget(const int ID, const Transform2D& Tranform2D)
{
    mID             = ID;
    mParentWidget   = nullptr;
    mThis           = nullptr;
    mTransform      = Tranform2D;
    mAnimationComp  = nullptr;
}

Widget::~Widget()
{
    DeleteAllChildrenWidgets();
    mThis = nullptr;
    mParentWidget = nullptr;
    mAnimationComp = nullptr;
}

void Widget::Init()
{
    // Adjust transform relative to parent Widget
    if (mParentWidget != nullptr)
    {
        // Position
        mTransform.Position.x += mParentWidget->GetPosition().x;
        mTransform.Position.y += mParentWidget->GetPosition().y;

        // Rotation
        mTransform.Rotation = WrapValue(mTransform.Rotation + mParentWidget->GetRotation(), 360);

        // Scale
        mTransform.Scale.x *= mParentWidget->GetScale().x;
        mTransform.Scale.y *= mParentWidget->GetScale().y;
    }
    size_t Size = mChildrenWidgets.size();
    for (uint32 i = 0; i < Size; ++i)
    {
        if (mChildrenWidgets[i] != nullptr)
            mChildrenWidgets[i]->Init();
    }
    if (mAnimationComp != nullptr)
        mAnimationComp->Init();
}

void Widget::Update(float Elapsed)
{
    size_t Size = mChildrenWidgets.size();
    for (uint32 i = 0; i < Size; ++i)
    {
        if (mChildrenWidgets[i] != nullptr)
            mChildrenWidgets[i]->Update(Elapsed);
    }
    if (mAnimationComp != nullptr)
    {
        mAnimationComp->Update(Elapsed);
    }
}

void Widget::Render()
{
    if (mAnimationComp != nullptr && mAnimationComp->GetCurrentImage() != nullptr)
    {
        Renderer::Instance()->SetColor(255, 255, 255, 255);
        Renderer::Instance()->SetBlendMode(Renderer::ALPHA);
        Renderer::Instance()->DrawImage(mAnimationComp->GetCurrentImage(), GetPosition().x, GetPosition().y, 0, mAnimationComp->GetCurrentImage()->GetWidth()*GetScale().x, mAnimationComp->GetCurrentImage()->GetHeight()*GetScale().y, GetRotation() + mAnimationComp->GetCurrentImage()->GetFixImageRotation());
    }
    size_t Size = mChildrenWidgets.size();
    for (uint32 i = 0; i < Size; ++i)
    {
        if (mChildrenWidgets[i] != nullptr)
            mChildrenWidgets[i]->Render();
    }
}

void Widget::AddAnimationComponent(Ptr<CAnimation> _Component)
{
    if (_Component != nullptr)
        mAnimationComp = _Component;
}

void Widget::AddChildWidget(Ptr<Widget> ChildWidget)
{
    if (ChildWidget != nullptr)
    {
        mChildrenWidgets.push_back(ChildWidget);
        ChildWidget->SetParentWidget(mThis);
    }
}

void Widget::DeleteAllChildrenWidgets()
{
    size_t Size = mChildrenWidgets.size();
    for (uint32 i = 0; i < Size; ++i)
    {
        mChildrenWidgets[i] = nullptr;
    }
    mChildrenWidgets.clear();
}