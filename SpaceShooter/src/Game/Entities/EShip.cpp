#include "../Components/HeadersComponents.h"
#include "../../Engine/Math.h"
#include "../UI/WLifebar.h"
#include "../World.h"
#include "../Scene.h"
#include "../../Engine/Audio/Audiosource.h"
#include "EShip.h"

Ptr<EShip> EShip::Create()
{
    Ptr<EShip> p = new EShip();
    p->mThis = p.UpCast<Entity>();
    return p;
}

Ptr<EShip> EShip::Create(const Transform2D& Tranform2D, float MaxLinearAcceleration, float MaxAngularAcceleration, float LinearInertiaSec, float AngularInertiaSec)
{
    Ptr<EShip> p = new EShip(Tranform2D, MaxLinearAcceleration, MaxAngularAcceleration, LinearInertiaSec, AngularInertiaSec);
    p->mThis = p.UpCast<Entity>();
    return p;
}

EShip::EShip(const Transform2D& Tranform2D, float MaxLinearAcceleration, float MaxAngularAcceleration, float LinearInertiaSec, float AngularInertiaSec)
{
    mType               = EntityType::ENTITY_SHIP;
    mPrimaryWeaponComp  = nullptr;
    mMovementControllerActivated = true;
    SetTransform(Tranform2D);
    SetTickMovement(true);
    SetMaxLinearAcc(MaxLinearAcceleration);
    SetMaxAngularAcc(MaxAngularAcceleration);
    SetLinearInertia(LinearInertiaSec);
    SetAngularInertia(AngularInertiaSec);
}

EShip::~EShip()
{
    mPrimaryWeaponComp = nullptr;
    mLifeBarWidget = nullptr;
    Entity::~Entity();
}

void EShip::Init()
{
    Entity::Init();
    GetPrimaryWeaponComp();
}

void EShip::Update(float elapsed)
{
    Entity::Update(elapsed);
}

void EShip::Render()
{
    Entity::Render();
}

void EShip::ReceiveDamage(int Amount)
{
    mLife = mLife - Amount >= 0 ? mLife - Amount : 0;
    if (mLifeBarWidget != nullptr)
    {
        mLifeBarWidget->SetCurrentLife(mLife);
    }
    if (!mLife)
    {
        // Death condition
        // Tell the scene who wins/loses
        Ptr<Scene> CurrentScene = World::Instance()->GetCurrentScene();
        if (CurrentScene != nullptr)
        {
            if (mType == EntityType::ENTITY_SHIPCONTROLLABLE)
                CurrentScene->GoToState(Scene::FINISH_1P_LOSE_STATE);
            else
                CurrentScene->GoToState(Scene::FINISH_1P_WIN_STATE);
        }
        mThis->Destroy();
    }
}

Ptr<CWeapon> EShip::GetPrimaryWeaponComp()
{
    if (mPrimaryWeaponComp == nullptr)
    {
        size_t Size = GetAllComponents().size();
        for(uint32 i = 0; i < Size; ++i)
        {
            if (GetAllComponents()[i].DownCast<CWeapon>() != nullptr)
            {
                mPrimaryWeaponComp = GetAllComponents()[i].DownCast<CWeapon>();
                break;
            }
        }
    }
    return mPrimaryWeaponComp;
}

vec2& EShip::GetProjectileSpawnPos()
{
    // Get the Projectile spawn position on Scene from the relative position to the Ship
    float VecLength = sqrt(mRelativeProjectileSpawnPos.x*mRelativeProjectileSpawnPos.x + mRelativeProjectileSpawnPos.y*mRelativeProjectileSpawnPos.y);
    vec2 RelativeProjectilePos = vec2(GetForwardVector().x*VecLength, GetForwardVector().y*VecLength);
    mProjectileSpawnPos = vec2(GetPosition().x + RelativeProjectilePos.x, GetPosition().y + RelativeProjectilePos.y);
    return mProjectileSpawnPos;
}

Ptr<CWeapon> EShip::GetSecondaryWeaponComp()
{
    if (mSecondaryWeaponComp == nullptr)
    {
        size_t Size = GetAllComponents().size();
        for (uint32 i = 0; i < Size; ++i)
        {
            if (GetAllComponents()[i].DownCast<CWeapon>() != nullptr && GetAllComponents()[i].DownCast<CWeapon>() != mPrimaryWeaponComp)
            {
                mSecondaryWeaponComp = GetAllComponents()[i].DownCast<CWeapon>();
                break;
            }
        }
    }
    return mSecondaryWeaponComp;
}

void EShip::SetLifeBarWidget(Ptr<WLifebar> LifeBar)
{
    if (LifeBar != nullptr)
    {
        mLifeBarWidget = LifeBar;
        mLifeBarWidget->SetMaxLife(mLife);
        mLifeBarWidget->SetCurrentLife(mLife);
    }
}