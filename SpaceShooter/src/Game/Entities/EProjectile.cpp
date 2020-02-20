#include "../Components/HeadersComponents.h"
#include "../../Engine/Audio/Audiosource.h"
#include "EProjectile.h"


Ptr<EProjectile> EProjectile::Create()
{
    Ptr<EProjectile> p = new EProjectile();
    p->mThis = p.UpCast<Entity>();
    return p;
}

Ptr<EProjectile> EProjectile::Create(const Transform2D& Tranform2D, float MaxLinearAcceleration, float MaxAngularAcceleration, float LinearInertiaSec, float AngularInertiaSec)
{
    Ptr<EProjectile> p = new EProjectile(Tranform2D, MaxLinearAcceleration, MaxAngularAcceleration, LinearInertiaSec, AngularInertiaSec);
    p->mThis = p.UpCast<Entity>();
    return p;
}

EProjectile::EProjectile(const Transform2D& Tranform2D, float MaxLinearAcceleration, float MaxAngularAcceleration, float LinearInertiaSec, float AngularInertiaSec)
{
    mType = EntityType::ENTITY_PROJECTILE;
    SetTransform(Tranform2D);
    SetTickMovement(true);
    SetMaxLinearAcc(MaxLinearAcceleration);
    SetMaxAngularAcc(MaxAngularAcceleration);
    SetLinearInertia(LinearInertiaSec);
    SetAngularInertia(AngularInertiaSec);
}

void EProjectile::Init()
{
    Entity::Init();
}

void EProjectile::Update(float elapsed)
{
    Entity::Update(elapsed);
}

void EProjectile::Render()
{
    Entity::Render();
}

EProjectile::ProjectileType EProjectile::GetProjectileTypeFromString(const String& Type)
{
    if (Type == "PROJECTILE_MISSILE_001")
        return EProjectile::PROJECTILE_MISSILE_001;
    else if (Type == "PROJECTILE_MISSILE_002")
        return EProjectile::PROJECTILE_MISSILE_002;
    else
        return EProjectile::PROJECTILE_MISSILE_001;
}