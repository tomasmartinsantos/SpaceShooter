#ifndef EASTEROID_H
#define EASTEROID_H

#include "../../Engine/Types.h"
#include "Entity.h"

class EAsteroid : public Entity
{
public:
    // Methods
    static Ptr<EAsteroid> Create();
    static Ptr<EAsteroid> Create(const Transform2D& Tranform2D, float MaxLinearAcceleration = 0.0f, float MaxAngularAcceleration = 0.0f, float LinearInertiaSec = 0.0f, float AngularInertiaSec = 0.0f);

    virtual void Init();
    virtual void Update(float elapsed);
    virtual void Render();

protected:
    EAsteroid() { mType = EntityType::ENTITY_ASTEROID; }
    EAsteroid(const Transform2D& Tranform2D, float MaxLinearAcceleration = 0.0f, float MaxAngularAcceleration = 0.0f, float LinearInertiaSec = 0.0f, float AngularInertiaSec = 0.0f);

    // Variables
private:

    friend class Ptr<EAsteroid>;
    friend class Ptr<const EAsteroid>;
};
#endif