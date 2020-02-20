#ifndef CBRAIN_H
#define CBRAIN_H

#include "../../Engine/Types.h"
#include "Component.h"

class BehaviorTree;
class EShip;

class CBrain : public Component
{
    // Methods
public:
    static Ptr<CBrain> Create(WPtr<Entity> _Entity, float SecondsToNewTarget, float RotationReachedValue, float PositionReachedValue);

    virtual void Init();
    virtual void Update(float Elapsed);


    Ptr<Entity> GetMyEntity() const { return mEntity; }
    bool        CheckMyEntityLimitPosition();

    void SelectNewTargetPosition();
    void RecalculateOpponentVars();
    const vec2& GetTargetPosition(bool IsAttacking)     const { if (IsAttacking) return mTargetPositionAttack; else return mTargetPositionMove; }
    const vec2& GetDirectionToTarget(bool IsAttacking)  const { if (IsAttacking) return mDirectionToTargetAttack; else return mDirectionToTargetMove; }
    float GetNormDirectionToTarget(bool IsAttacking)    const { if (IsAttacking) return mNormDirectionToTargetAttack; else return mNormDirectionToTargetMove; }

    float GetTargetRotation() const { return mTargetRotation; }
    void  SetTargetRotation(float Value) { mTargetRotation = Value; }

    bool  GetAILinearInertiaActivated() const { return mAILinearInertiaActivated; }
    void  SetAILinearInertiaActivated(bool Value) { mAILinearInertiaActivated = Value; }
    bool  GetAIAngularInertiaActivated() const { return mAIAngularInertiaActivated; }
    void  SetAIAngularInertiaActivated(bool Value) { mAIAngularInertiaActivated = Value; }

    Ptr<EShip> GetMyOpponent();

protected:
    CBrain();
    CBrain(WPtr<Entity> _Entity, float SecondsToNewTarget, float RotationReachedValue, float PositionReachedValue);
    virtual ~CBrain();

    // Variables
private:
    Ptr<BehaviorTree> mBehaviorTree;
    WPtr<EShip> mOpponent;

    float  mSecondsToNewTarget;
    float  mContSecondsNewTarget;

    float  mTargetRotationReached;
    float  mTargetPositionReached;

    vec2 mTargetPositionMove;
    vec2 mDirectionToTargetMove;
    float mNormDirectionToTargetMove;
    float mTargetRotation;

    vec2 mTargetPositionAttack;
    vec2 mDirectionToTargetAttack;
    float mNormDirectionToTargetAttack;

    bool mAILinearInertiaActivated;
    bool mAIAngularInertiaActivated;

    friend class Ptr<CBrain>;
    friend class Ptr<const CBrain>;
};

#endif