#ifndef CCOLLISION_H
#define CCOLLISION_H

#include "../../Engine/Types.h"
#include "Component.h"

class CCollision : public Component
{
    // Methods
public:
    static Ptr<CCollision> Create(WPtr<Entity> _Entity, float Radius, int Damage, Ptr<AudioSource> ExplosionSound);

    virtual void Init();
    virtual void Update(float elapsed);

    bool GetTickCollision() const     { return mTickCollision;}
    void SetTickCollision(bool Value) { mTickCollision = Value; }

    float GetCollisionRadius() const        { return mCollisionRadius; }
    void  SetCollisionRadius(float Radius)  { mCollisionRadius = Radius; }

    int GetCollisionDamage() const { return mCollisionDamage; }
    void  SetCollisionDamage(int Damage) { mCollisionDamage = Damage; }

    void Rebound();

    Ptr<Entity> GetMyEntity() const { return mEntity; }

    void ManageCollision(Ptr<CCollision> OtherComponent /*the component that has collided with me*/);

protected:
    CCollision();
    CCollision(WPtr<Entity> _Entity, float Radius, int Damage, Ptr<AudioSource> ExplosionSound);
    virtual ~CCollision();

    // Variables
private:
    bool  mTickCollision;

    float mCollisionRadius;
    int   mCollisionDamage;

    bool  mReboundActivated;
    float mReboundTimeCounter;
    float mReboundTime;
    /* How much seconds the component is not ticking collisions while rebounding */
    float mReboundTimeWithNoCollision;

    friend class Ptr<CCollision>;
    friend class Ptr<const CCollision>;
};

#endif