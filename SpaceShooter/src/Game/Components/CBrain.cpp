#include "../../Engine/IA/BehaviourTree.h"
#include "../IA_Behavior/Behaviors.h"
#include "../World.h"
#include "../Scene.h"
#include "../Entities/HeadersEntities.h"
#include "../../Engine/Math.h"
#include "../../Engine/Audio/Audiosource.h"
#include "../UI/WLifebar.h"
#include "CWeapon.h"
#include "CAnimation.h"
#include "CBrain.h"

Ptr<CBrain> CBrain::Create(WPtr<Entity> _Entity, float SecondsToNewTarget, float RotationReachedValue, float PositionReachedValue)
{
    if (_Entity != nullptr)
    {
        Ptr<CBrain> p = new CBrain(_Entity, SecondsToNewTarget, RotationReachedValue, PositionReachedValue);
        p->mThis = p.UpCast<Component>();
        return p;
    }
    else
        return nullptr;
}

CBrain::CBrain()
{
    mType                           = ComponentType::COMPONENT_BRAIN;
    mBehaviorTree                   = nullptr;
    mOpponent                       = nullptr;
    mSecondsToNewTarget             = 20.0f;
    mTargetRotationReached          = 20.0f;
    mTargetPositionReached          = 40.0f;
    mContSecondsNewTarget           = 0.0f;
    mTargetPositionMove             = vec2();
    mDirectionToTargetMove          = vec2();
    mNormDirectionToTargetMove      = 0.0f;
    mTargetPositionAttack           = vec2();
    mDirectionToTargetAttack        = vec2();
    mNormDirectionToTargetAttack    = 0.0f;
    mTargetRotation                 = 0.0f;
    mAILinearInertiaActivated       = false;
    mAIAngularInertiaActivated      = false;
}

CBrain::CBrain(WPtr<Entity> _Entity, float SecondsToNewTarget, float RotationReachedValue, float PositionReachedValue)
{
    mType                           = ComponentType::COMPONENT_BRAIN;
    mBehaviorTree                   = nullptr;
    mOpponent                       = nullptr;
    mEntity                         = _Entity;
    mSecondsToNewTarget             = SecondsToNewTarget;
    mTargetRotationReached          = RotationReachedValue;
    mTargetPositionReached          = PositionReachedValue;
    mContSecondsNewTarget           = 0.0f;
    mTargetPositionMove             = vec2();
    mDirectionToTargetMove          = vec2();
    mNormDirectionToTargetMove      = 0.0f;
    mTargetPositionAttack           = vec2();
    mDirectionToTargetAttack        = vec2();
    mNormDirectionToTargetAttack    = 0.0f;
    mTargetRotation                 = 0.0f;
    mAILinearInertiaActivated       = false;
    mAIAngularInertiaActivated      = false;
}

CBrain::~CBrain()
{
    mBehaviorTree   = nullptr;
    mOpponent       = nullptr;
    Component::~Component();
}

void CBrain::Init()
{
    Component::Init();
    // Initialize Brain Component
    if (World::IsEntityValid(mEntity))
    {
        mTargetPositionMove = mEntity->GetPosition();
        mTargetRotation     = mEntity->GetRotation();
    }

    if (World::IsEntityValid(GetMyOpponent().UpCast<Entity>()))
        RecalculateOpponentVars();
    
    // Create the behavior tree that will dictate the actions of the Enity
    mBehaviorTree = BehaviorTree::Create();
    if (mBehaviorTree != nullptr)
    {
        Ptr<Selector> SelectorRoot = Selector::Create();

        // IDLE
        Ptr<Sequence> SequenceIdle      = Sequence::Create();
        Ptr<Idle> Idle                  = Idle::Create(mThis.DownCast<CBrain>());
        if (SequenceIdle != nullptr)
        {
            SequenceIdle->AddBehavior(Idle.UpCast<Behavior>());
        }

        // MOVEMENT
        // Includes orientation to target before moving
        Ptr<Sequence>       SequenceMovement    = Sequence::Create();
        Ptr<WantToMove>     WantToDoMove        = WantToMove::Create(mThis.DownCast<CBrain>(), mTargetPositionReached);
        Ptr<RotateToTarget> RotateTo            = RotateToTarget::Create(mThis.DownCast<CBrain>(), false, mTargetRotationReached);
        Ptr<MoveToTarget>   MoveTo              = MoveToTarget::Create(mThis.DownCast<CBrain>(), mTargetPositionReached);
        if (SequenceMovement != nullptr)
        {
            SequenceMovement->AddBehavior(WantToDoMove.UpCast<Behavior>());
            SequenceMovement->AddBehavior(RotateTo.UpCast<Behavior>());
            SequenceMovement->AddBehavior(MoveTo.UpCast<Behavior>());
        }

        // ATTACK
        // Includes orientation to target before attacking 
        Ptr<Sequence>       SequenceAttack  = Sequence::Create();
        Ptr<RotateToTarget> TargetOpponent  = RotateToTarget::Create(mThis.DownCast<CBrain>(), true, mTargetRotationReached);
        Ptr<Attack>         Attack          = Attack::Create(mThis.DownCast<CBrain>());
        if (SequenceAttack != nullptr)
        {
            SequenceAttack->AddBehavior(TargetOpponent.UpCast<Behavior>());
            SequenceAttack->AddBehavior(Attack.UpCast<Behavior>());
        }

        if (SelectorRoot != nullptr)
        {
            SelectorRoot->AddBehavior(SequenceAttack.UpCast<Behavior>());
            SelectorRoot->AddBehavior(SequenceMovement.UpCast<Behavior>());
            SelectorRoot->AddBehavior(SequenceIdle.UpCast<Behavior>());
        }

        mBehaviorTree->SetRoot(SelectorRoot.UpCast<Group>());
        mBehaviorTree->Init();
    }
}

void CBrain::Update(float Elapsed)
{
    Component::Update(Elapsed);
    mContSecondsNewTarget += Elapsed;
    // Select a new target to move every SecondsToNewTarget
    if (mContSecondsNewTarget >= mSecondsToNewTarget)
        SelectNewTargetPosition();
    RecalculateOpponentVars();
    if(mBehaviorTree != nullptr)
        mBehaviorTree->Update(Elapsed);
}

bool CBrain::CheckMyEntityLimitPosition()
{
    // Check if my entity is in the Scene Limits
    Ptr<Scene> CurrentScene = World::Instance()->GetCurrentScene();
    if (World::IsEntityValid(GetMyEntity()) && CurrentScene != nullptr)
    {
        vec2 Pos = GetMyEntity()->GetPosition();
        if (Pos.x >= CurrentScene->GetSceneLimitMaxX() || Pos.x <= CurrentScene->GetSceneLimitMinX() || Pos.y >= CurrentScene->GetSceneLimitMaxY() || Pos.y <= CurrentScene->GetSceneLimitMinY())
        {
            return true;
        }
    }
    return false;
}

void CBrain::SelectNewTargetPosition()
{
    // Select a new target "close" to the opponent
    mContSecondsNewTarget = 0;
    Ptr<Scene> CurrentScene = World::Instance()->GetCurrentScene();
    if (CurrentScene != nullptr && World::IsEntityValid(GetMyEntity()) && World::IsEntityValid(GetMyOpponent().UpCast<Entity>()))
    {
        vec2    DirectionEnemyMyEntity = vec2(GetMyOpponent()->GetPosition().x - GetMyEntity()->GetPosition().x, GetMyOpponent()->GetPosition().y - GetMyEntity()->GetPosition().y);
        vec2    MidDirection = vec2(AbsValue(DirectionEnemyMyEntity.x) / 2, AbsValue(DirectionEnemyMyEntity.y) / 2);
        float   NormDirectionDirectionEnemyMyEntity = sqrtf(DirectionEnemyMyEntity.x*DirectionEnemyMyEntity.x + DirectionEnemyMyEntity.y*DirectionEnemyMyEntity.y);
        if (NormDirectionDirectionEnemyMyEntity > 0)
        {
            DirectionEnemyMyEntity.x = DirectionEnemyMyEntity.x / NormDirectionDirectionEnemyMyEntity;
            DirectionEnemyMyEntity.y = DirectionEnemyMyEntity.y / NormDirectionDirectionEnemyMyEntity;
        }
        vec2 OrthogonalDirection = vec2(-DirectionEnemyMyEntity.y, DirectionEnemyMyEntity.x);
        do
        {
            float NewX = (float) (rand() % 400 - 200);
            float NewY = (float) (rand() % 400 - 200);
            mTargetPositionMove = vec2(MidDirection.x + OrthogonalDirection.x*NewX, MidDirection.y + OrthogonalDirection.y*NewY);
        } while (mTargetPositionMove.x < (CurrentScene->GetSceneLimitMinX() + 50) || mTargetPositionMove.x >(CurrentScene->GetSceneLimitMaxX() - 50) || mTargetPositionMove.y < (CurrentScene->GetSceneLimitMinY() + 50) || mTargetPositionMove.y >(CurrentScene->GetSceneLimitMaxY() - 50));

    }

    if (World::IsEntityValid(GetMyEntity()))
    {
        // As well as the position to move, also calculate the Direction vector (normalized)
        mDirectionToTargetMove = vec2(mTargetPositionMove.x - GetMyEntity()->GetPosition().x, mTargetPositionMove.y - GetMyEntity()->GetPosition().y);
        mNormDirectionToTargetMove = sqrtf(mDirectionToTargetMove.x*mDirectionToTargetMove.x + mDirectionToTargetMove.y*mDirectionToTargetMove.y);
        if (mNormDirectionToTargetMove > 0)
        {
            mDirectionToTargetMove.x = mDirectionToTargetMove.x / mNormDirectionToTargetMove;
            mDirectionToTargetMove.y = mDirectionToTargetMove.y / mNormDirectionToTargetMove;
        }
    }
}

void CBrain::RecalculateOpponentVars()
{
    // Get the position and the direction from Entity to Opponent
    if (World::IsEntityValid(GetMyEntity()) && World::IsEntityValid(GetMyOpponent().UpCast<Entity>()))
    {
        mTargetPositionAttack           = GetMyOpponent()->GetPosition();
        mDirectionToTargetAttack        = vec2(mTargetPositionAttack.x - GetMyEntity()->GetPosition().x, mTargetPositionAttack.y - GetMyEntity()->GetPosition().y);
        mNormDirectionToTargetAttack    = sqrtf(mDirectionToTargetAttack.x*mDirectionToTargetAttack.x + mDirectionToTargetAttack.y*mDirectionToTargetAttack.y);
        if (mNormDirectionToTargetAttack > 0)
        {
            mDirectionToTargetAttack.x = mDirectionToTargetAttack.x / mNormDirectionToTargetAttack;
            mDirectionToTargetAttack.y = mDirectionToTargetAttack.y / mNormDirectionToTargetAttack;
        }
    }
}

Ptr<EShip> CBrain::GetMyOpponent()
{
    if (World::IsEntityValid(GetMyEntity()))
    {
        if (mOpponent == nullptr)
        {
            Ptr<Scene> CurrentScene = World::Instance()->GetCurrentScene();
            if (CurrentScene != nullptr)
                mOpponent = CurrentScene->GetOpponent(GetMyEntity().DownCast<EShip>());
        }
    }
    return mOpponent;
}