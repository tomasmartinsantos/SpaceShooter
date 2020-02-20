#include "../World.h"
#include "../Entities/HeadersEntities.h"
#include "../CollisionManager.h"
#include "../../Engine/Audio/Audiosource.h"
#include "../UI/WLifebar.h"
#include "CWeapon.h"
#include "CAnimation.h"
#include "../Scene.h"
#include "CCollision.h"


Ptr<CCollision> CCollision::Create(WPtr<Entity> _Entity, float Radius, int Damage, Ptr<AudioSource> ExplosionSound)
{
    if (_Entity != nullptr)
    {
        Ptr<CCollision> p = new CCollision(_Entity, Radius, Damage, ExplosionSound);
        p->mThis = p.UpCast<Component>();
        return p;
    }
    else
        return nullptr;
}

CCollision::CCollision()
{
    mType                       = ComponentType::COMPONENT_COLLISION;
    mTickCollision              = true;
    mCollisionRadius            = 0.0f;
    mCollisionDamage            = 0;
    mReboundActivated           = false;
    mReboundTimeCounter         = 0.0f;
    mReboundTime                = 1.0f;
    mReboundTimeWithNoCollision = 0.2f;
    mAudio                      = nullptr;
}

CCollision::CCollision(WPtr<Entity> _Entity, float Radius, int Damage, Ptr<AudioSource> ExplosionSound)
{
    mType                       = ComponentType::COMPONENT_COLLISION;
    mEntity                     = _Entity;
    mTickCollision              = true;
    mCollisionRadius            = Radius;
    mCollisionDamage            = Damage;
    mReboundActivated           = false;
    mReboundTimeCounter         = 0.0f;
    mReboundTime                = 1.0f;
    mReboundTimeWithNoCollision = 0.2f;
    mAudio                      = ExplosionSound;
}

CCollision::~CCollision()
{
    Component::~Component();
}

void CCollision::Init()
{
    Component::Init();
    CollisionManager::Instance()->AddCollisionComponent(mThis.DownCast<CCollision>());
}

void CCollision::Update(float elapsed)
{
    Component::Update(elapsed);
    // While the Entity is bounding, movement input is not allowed
    if (mReboundActivated)
    {
        mReboundTimeCounter += elapsed;
        if (mReboundTimeCounter >= mReboundTimeWithNoCollision)
            SetTickCollision(true);
        if (mReboundTimeCounter >= mReboundTime)
        {
            mReboundActivated = false;
            mReboundTimeCounter = 0.0f;
            if(World::IsEntityValid(GetMyEntity()) && GetMyEntity().DownCast<EShip>() != nullptr)
                GetMyEntity().DownCast<EShip>()->ActivateMovementController();
        }
    }
}

void CCollision::Rebound()
{
    // Set a linear steering on -forward direction to make the Entity "rebound"
    mReboundActivated = true;
    mReboundTimeCounter = 0.0f;
    SetTickCollision(false); //Stop ticking collision for a short period of time. This avoids "infinite colliding" in some specific and rare cases
    if (World::IsEntityValid(GetMyEntity()))
    {
        GetMyEntity()->DeactivateLinearInertia();
        GetMyEntity()->SetLinearSteering(vec2(-GetMyEntity()->GetLinearSteering().x, -GetMyEntity()->GetLinearSteering().y));
        if (GetMyEntity().DownCast<EShip>() != nullptr)
            GetMyEntity().DownCast<EShip>()->DeactivateMovementController();
    }
}

void CCollision::ManageCollision(Ptr<CCollision> OtherComponent)
{
    if (World::IsEntityValid(GetMyEntity()) && OtherComponent != nullptr)
    {
        // If Entity is a Ship, it always receives damage and rebounds if colliding with the other Ship or an Asteroid
        if (GetMyEntity().DownCast<EShip>() != nullptr)
        {
            GetMyEntity().DownCast<EShip>()->ReceiveDamage(OtherComponent->GetCollisionDamage());
            if (OtherComponent->GetMyEntity().DownCast<EAsteroid>() != nullptr)
                Rebound();
            else if (OtherComponent->GetMyEntity().DownCast<EShip>() != nullptr)
                Rebound();
        }

        // If Entity is a Projectile, stop movement, stop ticking collisions and start the destroy animation (explosion) + PlaySound
        else if (GetMyEntity().DownCast<EProjectile>() != nullptr)
        {
            GetMyEntity()->SetTickMovement(false);
            SetTickCollision(false);
            GetMyEntity()->ShowNextAnimation(true);
            if (mAudio != nullptr)
                mAudio->Play();
        }

        // If Entity is an Asteroid, stop ticking collisions and start the destroy animation (explosion) + PlaySound
        else if (GetMyEntity().DownCast<EAsteroid>() != nullptr)
        {
            SetTickCollision(false);
            GetMyEntity()->ShowNextAnimation(true);
            if (mAudio != nullptr)
                mAudio->Play();
        }
    }
}