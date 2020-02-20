#include "../Components/HeadersComponents.h"
#include "../../Engine/Renderer.h"
#include "../../Engine/Image.h"
#include "../../Engine/Math.h"
#include "../CollisionManager.h"
#include "../../Engine/Audio/Audiosource.h"
#include "Entity.h"


Ptr<Entity> Entity::Create()
{
    Ptr<Entity> p = new Entity();
    p->mThis = p;
    return p;
}

Ptr<Entity> Entity::Create(const Transform2D& Tranform2D, float MaxLinearAcceleration, float MaxAngularAcceleration, float LinearInertiaSec, float AngularInertiaSec)
{
    Ptr<Entity> p = new Entity(Tranform2D, MaxLinearAcceleration, MaxAngularAcceleration, LinearInertiaSec, AngularInertiaSec);
    p->mThis = p;
    return p;
}

Entity::Entity()
{
    mType                       = EntityType::ENTITY_BASIC;
    mThis                       = nullptr;
    mTransform                  = Transform2D();
    mForwardVector              = vec2();
    mPendingKill                = false;
    mAnimationComp              = nullptr;
    mTickMovement               = true;
    mMaxLinearAcceleration      = 0.0f;
    mLinearSteering             = vec2();
    mLinearVelocity             = vec2();
    mLinearInertiaSec           = 0.0f;
    mLinearInertiaCont          = 0.0f;
    mLinearInertiaActivated     = false;
    mTurnDirection              = false;
    mMaxAngularAcceleration     = 0.0f;
    mAngularVelocity            = 0.0f;
    mAngularSteering            = 0.0f;
    mAngularInertiaSec          = 0.0f;
    mAngularInertiaCont         = 0.0f;
    mAngularInertiaActivated    = false;
}

Entity::Entity(const Transform2D& Tranform2D, float MaxLinearAcceleration, float MaxAngularAcceleration, float LinearInertiaSec, float AngularInertiaSec)
{
    mType                       = EntityType::ENTITY_BASIC;
    mThis                       = nullptr;
    mTransform                  = Tranform2D;
    mForwardVector              = vec2();
    mPendingKill                = false;
    mAnimationComp              = nullptr;
    mTickMovement               = true;
    mMaxLinearAcceleration      = MaxLinearAcceleration;
    mLinearSteering             = vec2();
    mLinearVelocity             = vec2();
    mLinearInertiaSec           = LinearInertiaSec;
    mLinearInertiaCont          = 0.0f;
    mLinearInertiaActivated     = false;
    mTurnDirection              = false;
    mMaxAngularAcceleration     = MaxAngularAcceleration;
    mAngularVelocity            = 0.0f;
    mAngularSteering            = 0.0f;
    mAngularInertiaSec          = AngularInertiaSec;
    mAngularInertiaCont         = 0.0f;
    mAngularInertiaActivated    = false;
}

Entity::~Entity()
{
    DeleteAllComponents();
    mThis           = nullptr;
    mAnimationComp  = nullptr;
}

void Entity::Init()
{
    mForwardVector = vec2(-DegSin(GetRotation()), -DegCos(GetRotation()));
    size_t Size = mComponents.size();
    for(uint32 i = 0; i < Size; ++i)
    {
        if(mComponents[i] != nullptr)
            mComponents[i]->Init();
    }
}

void Entity::Update(float Elapsed)
{
    // First Update Entity Position and Rotation 
    if (mTickMovement)
    {
        UpdatePosition(Elapsed);
        UpdateRotation(Elapsed);
        mForwardVector = vec2(-DegSin(GetRotation()), -DegCos(GetRotation()));
    }
    // Then update all its components
    size_t Size = mComponents.size();
    for (uint32 i = 0; i < Size; ++i)
    {
        if (mComponents[i] != nullptr)
            mComponents[i]->Update(Elapsed);
    }
}

void Entity::Render()
{
    if (mAnimationComp != nullptr && mAnimationComp->GetCurrentImage() != nullptr)
    {
        Renderer::Instance()->SetColor(255, 255, 255, 255);
        Renderer::Instance()->SetBlendMode(Renderer::ALPHA);
        Renderer::Instance()->DrawImage(mAnimationComp->GetCurrentImage(), GetPosition().x, GetPosition().y, 0, mAnimationComp->GetCurrentImage()->GetWidth()*GetScale().x, mAnimationComp->GetCurrentImage()->GetHeight()*GetScale().y, GetRotation() + mAnimationComp->GetCurrentImage()->GetFixImageRotation());
    }
}

void Entity::AddComponent(Ptr<Component> _Component)
{
    if(_Component != nullptr)
        mComponents.push_back(_Component);
}

void Entity::DeleteAllComponents()
{
    size_t Size = mComponents.size();
    for (uint32 i = 0; i < Size; ++i)
    {
        if (mComponents[i].DownCast<CCollision>() != nullptr)
            CollisionManager::Instance()->RemoveCollisionComponent(mComponents[i].DownCast<CCollision>());
        mComponents[i] = nullptr;
    }
    mComponents.clear();
}

void Entity::ActivateLinearInertia()
{
    if (!mLinearInertiaActivated)
    {
        mLinearInertiaCont = 0.0f;
        mLinearInertiaActivated = true;
    }
}

void Entity::DeactivateLinearInertia()
{
    if (mLinearInertiaActivated)
    {
        mLinearInertiaCont = 0.0f;
        mLinearInertiaActivated = false;
    }
}

void Entity::ActivateAngularInertia()
{
    if (!mAngularInertiaActivated)
    {
        mAngularInertiaCont = 0.0f;
        mAngularInertiaActivated = true;
    }
}

void Entity::DeactivateAngularInertia()
{
    if (mAngularInertiaActivated)
    {
        mAngularInertiaCont = 0.0f;
        mAngularInertiaActivated = false;
    }
}

void Entity::UpdatePosition(float Elapsed)
{
    if (mLinearInertiaActivated)
    {
        mLinearInertiaCont += Elapsed;
        // If Still with inertia, set a linear steering (decelerate every tick)
        if (mLinearInertiaCont < mLinearInertiaSec)
        {
            float Acceleration = mMaxLinearAcceleration * ((mLinearInertiaSec - mLinearInertiaCont) / mLinearInertiaSec);
            SetLinearSteering(vec2(mForwardVector.x * Acceleration, mForwardVector.y * Acceleration));
        }
        // When time is over, stop
        else
        {
            mLinearInertiaCont = 0.0f;
            mLinearInertiaActivated = false;
            SetLinearSteering(vec2(0.0f, 0.0f));
        }
    }
    SetPosition(vec2(GetPosition().x + (mLinearVelocity.x + mLinearSteering.x * Elapsed)*Elapsed, GetPosition().y + (mLinearVelocity.y + mLinearSteering.y * Elapsed)*Elapsed));
}

void Entity::UpdateRotation(float Elapsed)
{
    if (mAngularInertiaActivated)
    {
        mAngularInertiaCont += Elapsed;
        // If Still with inertia, set an angular steering (decelerate every tick)
        if (mAngularInertiaCont < mAngularInertiaSec)
        {
            float Acceleration = mMaxAngularAcceleration * ((mAngularInertiaSec - mAngularInertiaCont) / mAngularInertiaSec);
            Acceleration *= mTurnDirection;
            SetAngularSteering(Acceleration);
        }
        // When time is over, stop
        else
        {
            mAngularInertiaCont = 0.0f;
            mAngularInertiaActivated = false;
            SetAngularSteering(0.0f);
        }
    }
    SetRotation(WrapValue(GetRotation() + (mAngularVelocity + mAngularSteering * Elapsed)*Elapsed, 360));
}

void Entity::ShowNextAnimation(bool IsDestroyAnimation)
{
    // On Animation Component, go to next animation and destroy the entity if this is the destroy animation
    if(mAnimationComp != nullptr)
        mAnimationComp->NextAnimation(IsDestroyAnimation);
}

Entity::EntityType Entity::GetEntTypeFromString(const String & Type)
{
    if (Type == "ENTITY_BASIC")
        return EntityType::ENTITY_BASIC;
    else if (Type == "ENTITY_SHIP")
        return EntityType::ENTITY_SHIP;
    else if (Type == "ENTITY_SHIPCONTROLLABLE")
        return EntityType::ENTITY_SHIPCONTROLLABLE;
    else if (Type == "ENTITY_PROJECTILE")
        return EntityType::ENTITY_PROJECTILE;
    else if (Type == "ENTITY_ASTEROID")
        return EntityType::ENTITY_ASTEROID;
    else
        return EntityType::ENTITY_BASIC;
}