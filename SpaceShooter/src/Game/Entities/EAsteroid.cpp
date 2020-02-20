#include "../Components/HeadersComponents.h"
#include "../../Engine/Audio/Audiosource.h"
#include "EAsteroid.h"

Ptr<EAsteroid> EAsteroid::Create()
{
    Ptr<EAsteroid> p = new EAsteroid();
    p->mThis = p.UpCast<Entity>();
    return p;
}

Ptr<EAsteroid> EAsteroid::Create(const Transform2D& Tranform2D, float MaxLinearAcceleration, float MaxAngularAcceleration, float LinearInertiaSec, float AngularInertiaSec)
{
    Ptr<EAsteroid> p = new EAsteroid(Tranform2D, MaxLinearAcceleration, MaxAngularAcceleration, LinearInertiaSec, AngularInertiaSec);
    p->mThis = p.UpCast<Entity>();
    return p;
}

EAsteroid::EAsteroid(const Transform2D& Tranform2D, float MaxLinearAcceleration, float MaxAngularAcceleration, float LinearInertiaSec, float AngularInertiaSec)
{
    mType = EntityType::ENTITY_ASTEROID;
    SetTransform(Tranform2D);
    SetTickMovement(false);
    SetMaxLinearAcc(MaxLinearAcceleration);
    SetMaxAngularAcc(MaxAngularAcceleration);
    SetLinearInertia(LinearInertiaSec);
    SetAngularInertia(AngularInertiaSec);
}

void EAsteroid::Init()
{
    Entity::Init();
}

void EAsteroid::Update(float elapsed)
{
    Entity::Update(elapsed);
}

void EAsteroid::Render()
{
    Entity::Render();
}