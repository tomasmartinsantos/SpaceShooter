#ifndef WIDGET_H
#define WIDGET_H

#include "../../Engine/Types.h"
#include "../../Engine/String.h"

class CAnimation;

class Widget      // Widget class has a Tree structure: a pointer to ParentWidget and a vector of ChildWidgets
{
    // Methods
public:
    static Ptr<Widget> Create();
    static Ptr<Widget> Create(const int ID, const Transform2D& Tranform2D);

    virtual void Init();
    virtual void Update(float elapsed);
    virtual void Render();

    int GetID()                     const { return mID;}

    Transform2D&    GetTransform()        { return mTransform; }
    const vec2&     GetPosition()   const { return mTransform.Position; }
    float           GetRotation()   const { return mTransform.Rotation; }
    const vec2&     GetScale()      const { return mTransform.Scale; }

    void SetTransform(const Transform2D& Tranform2D) { mTransform = Tranform2D; }
    void SetPosition(vec2& Position) { mTransform.Position = Position; }
    void SetRotation(float Rotation) { mTransform.Rotation = Rotation; }
    void SetScale(vec2& Scale) { mTransform.Scale = Scale; }

    void SetParentWidget(Ptr<Widget> ParentWidget) { mParentWidget = ParentWidget; }

    void AddAnimationComponent(Ptr<CAnimation> _Component);

    void AddChildWidget(Ptr<Widget> ChildWidget);
    void DeleteAllChildrenWidgets();

protected:
    Widget();
    Widget(const int ID, const Transform2D& Tranform2D);
    virtual ~Widget();

    // Variables
protected:
    WPtr<Widget>                mThis;
    WPtr<Widget>                mParentWidget;
    int                         mID;
    Ptr<CAnimation>             mAnimationComp;

private:
    Transform2D                 mTransform;
    std::vector<Ptr<Widget>>    mChildrenWidgets;


    friend class Ptr<Widget>;
    friend class Ptr<const Widget>;
};

#endif