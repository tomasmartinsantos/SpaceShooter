#include "../Entities/HeadersEntities.h"
#include "../World.h"
#include "../Scene.h"
#include "../../Engine/Audio/Audiosource.h"
#include "../UI/WLifebar.h"
#include "CAnimation.h"
#include "CWeapon.h"


Ptr<CWeapon> CWeapon::Create(WPtr<Entity> _Entity, int WeaponCollectionIndex, float CooldownSecs, Ptr<AudioSource> FireSound)
{
    if (_Entity != nullptr)
    {
        Ptr<CWeapon> p = new CWeapon(_Entity, WeaponCollectionIndex, CooldownSecs, FireSound);
        p->mThis = p.UpCast<Component>();
        return p;
    }
    else
        return nullptr;
}

CWeapon::CWeapon()
{
    mType                       = ComponentType::COMPONENT_WEAPON; 
    mProjectileCollectionIndex  = -1;
    mCooldownSecs               = 0.0f;
    mCooldownCont               = 0.0f;
    mAudio                      = nullptr;
}

CWeapon::CWeapon(WPtr<Entity> _Entity, int WeaponCollectionIndex, float CooldownSecs, Ptr<AudioSource> FireSound)
{
    mType                       = ComponentType::COMPONENT_WEAPON;
    mEntity                     = _Entity;
    mProjectileCollectionIndex  = WeaponCollectionIndex;
    mCooldownSecs               = CooldownSecs;
    mCooldownCont               = 0.0f;
    mAudio                      = FireSound;
}

CWeapon::~CWeapon()
{
    Component::~Component();
}

void CWeapon::Init()
{
    Component::Init();
}

void CWeapon::Update(float Elapsed)
{
    Component::Update(Elapsed);
    if (mIsInCooldown)
    {
        // Update Weapon cooldown
        mCooldownCont += Elapsed;
        if (mCooldownCont >= mCooldownSecs)
        {
            mIsInCooldown = false;
            mCooldownCont = 0.0f;
        }
    }
}

void CWeapon::Fire(vec2 Position, float Rotation)
{
    if (!mIsInCooldown)
    {
        Ptr<Scene> CurrentScene = World::Instance()->GetCurrentScene();
        if (CurrentScene != nullptr)
        {
            // Spawn the Projectile, based on a Projectile prototype (copying all its parameters)
            Ptr<Entity> NewProjectile = CurrentScene->CopyEntity(CurrentScene->GetProjectileInCollection(mProjectileCollectionIndex).UpCast<Entity>());
            if (World::IsEntityValid(NewProjectile))
            {
                NewProjectile->SetPosition(Position);
                NewProjectile->SetRotation(Rotation);
                NewProjectile->ActivateLinearInertia();
                if (NewProjectile.DownCast<EProjectile>() != nullptr)
                {
                    NewProjectile.DownCast<EProjectile>()->SetProjectileType(CurrentScene->GetProjectileInCollection(mProjectileCollectionIndex)->GetProjectileType());
                    mIsInCooldown = true; // Start Cooldown and PlaySound
                    if (mAudio != nullptr)
                        mAudio->Play();
                }
            }
        }
    }
}