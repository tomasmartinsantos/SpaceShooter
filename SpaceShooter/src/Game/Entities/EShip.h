#ifndef ESHIP_H
#define ESHIP_H

#include "../../Engine/Types.h"
#include "Entity.h"

class CWeapon;
class WLifebar;

class EShip : public Entity
{
    // Methods
public:
    static Ptr<EShip> Create();
    static Ptr<EShip> Create(const Transform2D& Tranform2D, float MaxLinearAcceleration = 0.0f, float MaxAngularAcceleration = 0.0f, float LinearInertiaSec = 0.0f, float AngularInertiaSec = 0.0f);

    virtual void Init();
    virtual void Update(float elapsed);
    virtual void Render();

    int     GetLife() const { return mLife; }
    void    SetLife(int Life) { mLife = Life; }

    void    ReceiveDamage(int Amount);

    Ptr<CWeapon>    GetPrimaryWeaponComp();
    void            SetPrimaryWeaponComp(Ptr<CWeapon> WeaponComp, vec2& RelativeSpawnPos) { mPrimaryWeaponComp = WeaponComp; SetRelativeProjectileSpawnPos(RelativeSpawnPos); }

    vec2& GetProjectileSpawnPos();
    void SetRelativeProjectileSpawnPos(vec2& Position) { mRelativeProjectileSpawnPos = Position; }

    bool IsPrimaryWeaponActive() const { return mIsPrimaryWeaponActive; }
    void SetPrimaryWeaponActive(bool Value) { mIsPrimaryWeaponActive = Value; }

    Ptr<CWeapon>    GetSecondaryWeaponComp();
    void            SetSecondaryWeaponComp(Ptr<CWeapon> WeaponComp, vec2& RelativeSpawnPos) { mSecondaryWeaponComp = WeaponComp; SetRelativeProjectileSpawnPos(RelativeSpawnPos); }


    bool IsSecondaryWeaponActive() const { return mIsSecondaryWeaponActive; }
    void SetSecondaryWeaponActive(bool Value) { mIsSecondaryWeaponActive = Value; }

    Ptr<WLifebar> GetLifeBarWidget() const { return mLifeBarWidget; }
    void SetLifeBarWidget(Ptr<WLifebar> LifeBar);

    void DeactivateMovementController() { mMovementControllerActivated = false; }
    void ActivateMovementController() { mMovementControllerActivated = true; }
    bool IsMovementControllerActivated() const { return mMovementControllerActivated; }

protected:
    EShip() { mType = EntityType::ENTITY_SHIP; mPrimaryWeaponComp  = nullptr; }
    EShip(const Transform2D& Tranform2D, float MaxLinearAcceleration = 0.0f, float MaxAngularAcceleration = 0.0f, float LinearInertiaSec = 0.0f, float AngularInertiaSec = 0.0f);
    virtual ~EShip();

    // Variables
protected:
    Ptr<CWeapon>    mPrimaryWeaponComp;
    Ptr<CWeapon>    mSecondaryWeaponComp;
    Ptr<WLifebar>   mLifeBarWidget;

private:
    int  mLife;

    vec2 mRelativeProjectileSpawnPos;
    vec2 mProjectileSpawnPos;
    bool mIsPrimaryWeaponActive;
    bool mIsSecondaryWeaponActive;

    bool mMovementControllerActivated;

    friend class Ptr<EShip>;
    friend class Ptr<const EShip>;
};

#endif