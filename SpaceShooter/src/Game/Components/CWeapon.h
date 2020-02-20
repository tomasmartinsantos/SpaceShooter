#ifndef CWEAPON_H
#define CWEAPON_H

#include "../../Engine/Types.h"
#include "Component.h"


class CWeapon : public Component
{
    // Methods
public:
    static Ptr<CWeapon> Create(WPtr<Entity> _Entity, int WeaponCollectionIndex, float CooldownSecs, Ptr<AudioSource> FireSound);

    virtual void Init();
    virtual void Update(float Elapsed);

    int GetProjectileCollectionIndex() const { return mProjectileCollectionIndex; }
    void Fire(vec2 Position, float Rotation);
    float GetCooldownSecs() const { return mCooldownSecs; }
    bool IsWeaponInCooldown() const { return mIsInCooldown; }

protected:
    CWeapon();
    CWeapon(WPtr<Entity> _Entity, int WeaponCollectionIndex, float CooldownSecs, Ptr<AudioSource> FireSound);
    virtual ~CWeapon();

    // Variables
private:
    int     mProjectileCollectionIndex;
    float   mCooldownSecs;
    float   mCooldownCont;
    bool    mIsInCooldown;

    friend class Ptr<CWeapon>;
    friend class Ptr<const CWeapon>;
};

#endif