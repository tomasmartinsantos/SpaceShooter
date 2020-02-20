#include "../World.h"
#include "../../Engine/Image.h"
#include "../Entities/Entity.h"
#include "../../Engine/Audio/Audiosource.h"
#include "../Scene.h"
#include "CAnimation.h"


Ptr<CAnimation> CAnimation::Create(WPtr<Entity> _Entity, const std::vector<std::pair<int, std::vector<Ptr<Image>>>>& Animations)
{
    Ptr<CAnimation> p = new CAnimation(_Entity, Animations);
    p->mThis = p.UpCast<Component>();
    return p;
}

CAnimation::CAnimation()
{
    mType               = ComponentType::COMPONENT_ANIMATION;
    mIsDestroyAnimation = false;
    mCurrentAnimation   = 0;
    mCurrentFrame       = 0;
    mNumberOfFrames     = 0;
    mSecPerImage        = 0.0f;
    mContTime           = 0.0f;
}

CAnimation::CAnimation(WPtr<Entity> _Entity, const std::vector<std::pair<int, std::vector<Ptr<Image>>>>& Animations)
{
    mType               = ComponentType::COMPONENT_ANIMATION;
    mEntity             = _Entity;
    mAnimations         = Animations;
    mIsDestroyAnimation = false;
    mCurrentAnimation   = 0;
    RecalculateAnimParams(0);
}

CAnimation::~CAnimation()
{
    RemoveAllAnimations();
    Component::~Component();
}

void CAnimation::Init()
{
    Component::Init();
    if (World::IsEntityValid(mEntity))
        mEntity->SetAnimComponent(mThis.DownCast<CAnimation>());
    size_t Size = mAnimations.size();
    for (uint32 i = 0; i < Size; ++i)
    {
        size_t Size2 = mAnimations[i].second.size();
        for(uint32 j = 0; j < Size2; ++j)
        {
            if (mAnimations[i].second[j] != nullptr && World::IsEntityValid(mEntity) /* Only SetMidHandle if is a component of an entity (not a Widget)*/)
                mAnimations[i].second[j]->SetMidHandle();
        }
    }
}

void CAnimation::Update(float elapsed)
{
    Component::Update(elapsed);
    // Update the current frame of the animation
    mContTime += elapsed;
    if (mContTime >= mSecPerImage)
    {
        int NextFrame = mCurrentFrame + 1;
        mCurrentFrame = NextFrame % mNumberOfFrames;
        mContTime = 0.0f;
        if (mIsDestroyAnimation && NextFrame == mNumberOfFrames && World::IsEntityValid(mEntity))
            mEntity->Destroy();
    }
}

void CAnimation::RecalculateAnimParams(int AnimIndex)
{
    mCurrentFrame = 0;
    mNumberOfFrames = mAnimations[AnimIndex].second.size();
    mSecPerImage = 1.0f / mAnimations[AnimIndex].first;
    mContTime = 0.0f;
}

void CAnimation::NextAnimation(bool IsDestroyAnimation)
{
    mIsDestroyAnimation = IsDestroyAnimation;
    mCurrentAnimation = mCurrentAnimation + 1 < ((int)mAnimations.size()) ? mCurrentAnimation + 1 : mCurrentAnimation;
    RecalculateAnimParams(mCurrentAnimation);
}


Ptr<Image> CAnimation::GetCurrentImage()
{
    return mAnimations[mCurrentAnimation].second[mCurrentFrame];
}

void CAnimation::RemoveAllAnimations()
{
    size_t Size = mAnimations.size();
    for (uint32 i = 0; i < Size; i++)
    {
        size_t Size2 = mAnimations[i].second.size();
        for (uint32 j = 0; j < Size2; ++j)
        {
            mAnimations[i].second[j] = nullptr;
        }
        mAnimations[i].second.clear();
    }
}