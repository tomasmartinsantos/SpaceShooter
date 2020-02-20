#ifndef EPROJECTILE_H
#define EPROJECTILE_H

#include "../../Engine/Types.h"
#include "Entity.h"

class EProjectile : public Entity
{

public:
    enum ProjectileType
    {
        PROJECTILE_MISSILE_001,
        PROJECTILE_MISSILE_002
    };
    // Methods
    static Ptr<EProjectile> Create();
    static Ptr<EProjectile> Create(const Transform2D& Tranform2D, float MaxLinearAcceleration = 0.0f, float MaxAngularAcceleration = 0.0f, float LinearInertiaSec = 0.0f, float AngularInertiaSec = 0.0f);

    virtual void Init();
    virtual void Update(float elapsed);
    virtual void Render();

    ProjectileType GetProjectileType() const { return mProjectileType; }
    void SetProjectileType(ProjectileType Type) { mProjectileType = Type; }


    static EProjectile::ProjectileType GetProjectileTypeFromString(const String& Type);

protected:
    EProjectile() { mType = EntityType::ENTITY_PROJECTILE; }
    EProjectile(const Transform2D& Tranform2D, float MaxLinearAcceleration = 0.0f, float MaxAngularAcceleration = 0.0f, float LinearInertiaSec = 0.0f, float AngularInertiaSec = 0.0f);

    // Variables
private:
    ProjectileType mProjectileType;

    friend class Ptr<EProjectile>;
    friend class Ptr<const EProjectile>;
};

#endif