#ifndef CANIMATION_H
#define CANIMATION_H

#include "../../Engine/Types.h"
#include "Component.h"

class Image;

class CAnimation : public Component
{
    // Methods
public:
    static Ptr<CAnimation> Create(WPtr<Entity> _Entity, const std::vector<std::pair<int, std::vector<Ptr<Image>>>>& Animations);

    virtual void Init();
    virtual void Update(float elapsed);

    Ptr<Image> GetCurrentImage();
    const std::vector<std::pair<int, std::vector<Ptr<Image>>>>& GetAllAnimations() const{ return mAnimations; }

    void NextAnimation(bool IsDestroyAnimation);
    void RecalculateAnimParams(int AnimIndex);

    void RemoveAllAnimations();

    int GetFPS() const      { return mAnimations[mCurrentAnimation].first;}
    void SetFPS(int FPS)    { mAnimations[mCurrentAnimation].first = FPS; }

protected:
    CAnimation();
    CAnimation(WPtr<Entity> _Entity, const std::vector<std::pair<int, std::vector<Ptr<Image>>>>& Animations);
    virtual ~CAnimation();

    // Variables
private:
    std::vector<std::pair<int, std::vector<Ptr<Image>>>> mAnimations;
    bool mIsDestroyAnimation;
    int mCurrentAnimation, mCurrentFrame, mNumberOfFrames;
    float mSecPerImage, mContTime;

    friend class Ptr<CAnimation>;
    friend class Ptr<const CAnimation>;
};

#endif