#include "../World.h"
#include "../../Engine/Math.h"
#include "../Components/CBrain.h"
#include "../Entities/HeadersEntities.h"
#include "../Components/CWeapon.h"
#include "../UI/WLifebar.h"
#include "../Components/CAnimation.h"
#include "../../Engine/Audio/Audiosource.h"
#include "../Scene.h"
#include "Behaviors.h"

/* BEHAVIORS */


// ATTACK
Ptr<Attack> Attack::Create(Ptr<CBrain> BrainComponent)
{
    return new Attack(BrainComponent);
}

Attack::Attack(Ptr<CBrain> BrainComponent)
{
    mOwnerBrain = BrainComponent;
    mStatus = eInvalid;
}

Attack::~Attack()
{
    mOwnerBrain = nullptr;
}

Status Attack::Update(float Step)
{
    // Try to attack with the secondary weapon (more powerful)
    // If not, then try to attack with the primary weapon
    if (mOwnerBrain != nullptr && World::IsEntityValid(mOwnerBrain->GetMyEntity()) && mOwnerBrain->GetMyEntity().DownCast<EShip>() != nullptr)
    {
        if (mOwnerBrain->GetMyEntity().DownCast<EShip>()->GetSecondaryWeaponComp() != nullptr && !mOwnerBrain->GetMyEntity().DownCast<EShip>()->GetSecondaryWeaponComp()->IsWeaponInCooldown())
        {
            mOwnerBrain->GetMyEntity().DownCast<EShip>()->GetSecondaryWeaponComp()->Fire(mOwnerBrain->GetMyEntity().DownCast<EShip>()->GetProjectileSpawnPos(), mOwnerBrain->GetMyEntity()->GetRotation());
            return eSuccess;
        }
        else if(mOwnerBrain->GetMyEntity().DownCast<EShip>()->GetPrimaryWeaponComp() != nullptr && !mOwnerBrain->GetMyEntity().DownCast<EShip>()->GetPrimaryWeaponComp()->IsWeaponInCooldown())
        {
            mOwnerBrain->GetMyEntity().DownCast<EShip>()->GetPrimaryWeaponComp()->Fire(mOwnerBrain->GetMyEntity().DownCast<EShip>()->GetProjectileSpawnPos(), mOwnerBrain->GetMyEntity()->GetRotation());
            return eSuccess;
        }
        else
            return eFail;
    }
    else
    {
        // Weapon in Cooldown
        return eFail;
    }
    return eInvalid;
}

// MOVEMENT
Ptr<WantToMove> WantToMove::Create(Ptr<CBrain> BrainComponent, float TargetReachedRadius)
{
    return new WantToMove(BrainComponent, TargetReachedRadius);
}

WantToMove::WantToMove(Ptr<CBrain> BrainComponent, float TargetReachedRadius)
{
    mOwnerBrain = BrainComponent;
    mTargetReachedRadius = TargetReachedRadius;
    mStatus = eInvalid;
}

WantToMove::~WantToMove()
{
    mOwnerBrain = nullptr;
}

Status WantToMove::Update(float Step)
{
    // Only move if not inside the acceptance radius
    if (mOwnerBrain != nullptr  && World::IsEntityValid(mOwnerBrain->GetMyEntity()) && mOwnerBrain->GetMyEntity().DownCast<EShip>() != nullptr && mOwnerBrain->GetMyEntity().DownCast<EShip>()->IsMovementControllerActivated())
    {
        if (SqrDistance(mOwnerBrain->GetMyEntity()->GetPosition(), mOwnerBrain->GetTargetPosition(false)) <= powf(mTargetReachedRadius, 2))
            return eFail;

        else
            return eSuccess;
    }
    return eInvalid;
}

Ptr<MoveToTarget> MoveToTarget::Create(Ptr<CBrain> BrainComponent, float TargetReachedRadius)
{
    return new MoveToTarget(BrainComponent, TargetReachedRadius);
}

MoveToTarget::MoveToTarget(Ptr<CBrain> BrainComponent, float TargetReachedRadius)
{
    mOwnerBrain = BrainComponent;
    mTargetReachedRadius = TargetReachedRadius;
    mStatus = eInvalid;
    mAbortBehaviorTimer = 10.0f;
}

MoveToTarget::~MoveToTarget()
{
    mOwnerBrain = nullptr;
}

Status MoveToTarget::Update(float Step)
{
    if (mOwnerBrain != nullptr)
    {
        // In case the MoveTo behavior went wrong, abort it
        if (mContAbortBehaviorTimer >= mAbortBehaviorTimer)
        {
            mOwnerBrain->SelectNewTargetPosition();
            return eInvalid;
        }
        else if (World::IsEntityValid(mOwnerBrain->GetMyEntity()) && mOwnerBrain->GetMyEntity().DownCast<EShip>() != nullptr && mOwnerBrain->GetMyEntity().DownCast<EShip>()->IsMovementControllerActivated())
        {
            // Check if entity is inside the acceptance radius
            if (SqrDistance(mOwnerBrain->GetMyEntity()->GetPosition(), mOwnerBrain->GetTargetPosition(false)) <= powf(mTargetReachedRadius, 2))
            {
                // If inside acceptance radius, activate the linear inertia to decelerate
                if (!mOwnerBrain->GetAILinearInertiaActivated())
                {
                    mOwnerBrain->SetAILinearInertiaActivated(true);
                    mOwnerBrain->GetMyEntity()->ActivateLinearInertia();
                }
                return eSuccess;
            }
            else
            {
                // Set a linear steering on forward vector
                mOwnerBrain->SetAILinearInertiaActivated(false);
                mOwnerBrain->GetMyEntity()->DeactivateLinearInertia();
                mOwnerBrain->GetMyEntity()->SetLinearSteering(vec2(mOwnerBrain->GetMyEntity()->GetForwardVector().x * mOwnerBrain->GetMyEntity()->GetMaxLinearAcc(), mOwnerBrain->GetMyEntity()->GetForwardVector().y * mOwnerBrain->GetMyEntity()->GetMaxLinearAcc()));
                return eRunning;
            }
        }
    }
    return eInvalid;
}

// TARGETING
Ptr<RotateToTarget> RotateToTarget::Create(Ptr<CBrain> BrainComponent, bool TargetingOpponent, float TargetReachedValue)
{
    return new RotateToTarget(BrainComponent, TargetingOpponent, TargetReachedValue);
}

RotateToTarget::RotateToTarget(Ptr<CBrain> BrainComponent, bool TargetingOpponent, float TargetReachedValue)
{
    mOwnerBrain = BrainComponent;
    mTargetReachedValue = TargetReachedValue;
    mStatus = eInvalid;
    mTargetingOpponent = TargetingOpponent;
}

RotateToTarget::~RotateToTarget()
{
    mOwnerBrain = nullptr;
}

Status RotateToTarget::Update(float Step)
{
    if (mTargetingOpponent && !World::IsEntityValid(mOwnerBrain->GetMyOpponent().UpCast<Entity>()))
    {
        return eInvalid;
    }
    if (mOwnerBrain != nullptr && World::IsEntityValid(mOwnerBrain->GetMyEntity()))
    {
        if (mOwnerBrain->GetMyEntity().DownCast<EShip>() != nullptr && mOwnerBrain->GetMyEntity().DownCast<EShip>()->IsMovementControllerActivated())
        {
            if (mOwnerBrain->GetNormDirectionToTarget(mTargetingOpponent) > 0)
            {
                // Calculate the angle from entity to target
                float NewAngle = DegACos(mOwnerBrain->GetMyEntity()->GetForwardVector().x * mOwnerBrain->GetDirectionToTarget(mTargetingOpponent).x + mOwnerBrain->GetMyEntity()->GetForwardVector().y * mOwnerBrain->GetDirectionToTarget(mTargetingOpponent).y);
                if (NewAngle < mTargetReachedValue)
                {
                    // If angle is minor acceptance value, activate angular inertia
                    mOwnerBrain->GetMyEntity()->SetAngularSteering(0.0f);
                    if (!mOwnerBrain->GetAIAngularInertiaActivated())
                    {
                        mOwnerBrain->SetAIAngularInertiaActivated(true);
                        mOwnerBrain->GetMyEntity()->ActivateAngularInertia();
                    }
                    return eSuccess;
                }
                else
                {
                    // In case the entity has to turn, first get the turn direction (left/right) and then set an angular steering
                    float CrossProd = mOwnerBrain->GetMyEntity()->GetForwardVector().x * mOwnerBrain->GetDirectionToTarget(mTargetingOpponent).y - mOwnerBrain->GetMyEntity()->GetForwardVector().y * mOwnerBrain->GetDirectionToTarget(mTargetingOpponent).x;
                    int8 TurnDirection = 1;
                    if (CrossProd > 0)
                        TurnDirection = -1;
                    mOwnerBrain->GetMyEntity()->SetTurnDirection(TurnDirection);
                    mOwnerBrain->GetMyEntity()->DeactivateAngularInertia();
                    mOwnerBrain->SetAIAngularInertiaActivated(false);
                    mOwnerBrain->GetMyEntity()->SetAngularSteering(TurnDirection * mOwnerBrain->GetMyEntity()->GetMaxAngularAcc());
                    return eRunning;
                }
            }
        }
    }
    return eInvalid;
}

// IDLE
Ptr<Idle> Idle::Create(Ptr<CBrain> BrainComponent)
{
    return new Idle(BrainComponent);
}

Idle::Idle(Ptr<CBrain> BrainComponent)
{
    mOwnerBrain = BrainComponent;
    mStatus = eInvalid;
}

Idle::~Idle()
{
    mOwnerBrain = nullptr;
}

Status Idle::Update(float Step)
{
    return eSuccess;
}