#ifndef ENTITY_H
#define ENTITY_H

#include "../../Engine/Types.h"
#include "../../Engine/String.h"

class Component;
class Image;
class CAnimation;

class Entity
{
    // Methods
public:
    enum EntityType
    {
        ENTITY_BASIC,
        ENTITY_SHIP,
        ENTITY_SHIPCONTROLLABLE,
        ENTITY_PROJECTILE,
        ENTITY_ASTEROID
    };
    static Ptr<Entity> Create();
    static Ptr<Entity> Create(const Transform2D& Tranform2D, float MaxLinearAcceleration = 0.0f, float MaxAngularAcceleration = 0.0f, float LinearInertiaSec = 0.0f, float AngularInertiaSec = 0.0f);

    virtual void Init();
    virtual void Update(float elapsed);
    virtual void Render();

    void AddComponent(Ptr<Component> _Component);
    std::vector<Ptr<Component>>& GetAllComponents() { return mComponents; }
    void DeleteAllComponents();

    bool IsPendingKill() const { return mPendingKill; }
    void Destroy() { mPendingKill = true; }

    EntityType GetType() const { return mType; }

    void SetAnimComponent(Ptr<CAnimation> AnimComp) { mAnimationComp = AnimComp; }
    void ShowNextAnimation(bool IsDestroyAnimation);

          Transform2D&    GetTransform()        { return mTransform; }
    const vec2&           GetPosition()   const { return mTransform.Position; }
    float                 GetRotation()   const { return mTransform.Rotation; }
    const vec2&           GetScale()      const { return mTransform.Scale; }

    void SetTransform(const Transform2D& Tranform2D)  { mTransform = Tranform2D; }
    void SetPosition(const vec2& Position)            { mTransform.Position = Position; }
    void SetRotation(float Rotation)            { mTransform.Rotation = Rotation; }
    void SetScale(const vec2& Scale)                  { mTransform.Scale = Scale; }

    const vec2&           GetForwardVector()      const { return mForwardVector; }
    void SetForwardVector(const vec2& ForwardVector)  { mForwardVector = ForwardVector;}


    // Movement
    bool  GetTickMovement() const               { return mTickMovement; }
    void  SetTickMovement(bool Value)           { mTickMovement = Value;}
    // Linear
    float GetMaxLinearAcc()               const { return mMaxLinearAcceleration;}
    float GetLinearInertia()              const { return mLinearInertiaSec; }
    void SetMaxLinearAcc(float Value)           { mMaxLinearAcceleration = Value; }
    void SetLinearSteering(const vec2& Value)   { mLinearSteering = Value; }
    const vec2& GetLinearSteering() const       { return mLinearSteering;}
    void SetLinearInertia(float Value)          { mLinearInertiaSec = Value; }
    void ActivateLinearInertia();
    void DeactivateLinearInertia();
    void UpdatePosition(float Elapsed);
    // Angular
    int8 GetTurnDirection()                                                 const { return mTurnDirection; }
    float GetMaxAngularAcc()                                                const { return mMaxAngularAcceleration; }
    float GetAngularInertia()                                               const { return mAngularInertiaSec; }
    void SetTurnDirection(int8 Value /*1 = left turn, -1 = right turn*/)          { mTurnDirection = Value; }
    void SetMaxAngularAcc(float Value)                                            { mMaxAngularAcceleration = Value; }
    void SetAngularSteering(float Value)                                          { mAngularSteering = Value; }
    void SetAngularInertia(float Value)                                           { mAngularInertiaSec = Value; }
    void ActivateAngularInertia();
    void DeactivateAngularInertia();
    void UpdateRotation(float Elapsed);

    static Entity::EntityType GetEntTypeFromString(const String& Type);

protected:
    Entity();
    Entity(const Transform2D& Tranform2D, float MaxLinearAcceleration = 0.0f, float MaxAngularAcceleration = 0.0f, float LinearInertiaSec = 0.0f, float AngularInertiaSec = 0.0f);
    virtual ~Entity();

    // Variables
protected:
    WPtr<Entity>                mThis;
    EntityType                  mType;

private:
    Transform2D                 mTransform;
    vec2                        mForwardVector;
    std::vector<Ptr<Component>> mComponents;
    bool                        mPendingKill;

    Ptr<CAnimation>             mAnimationComp;


    // Movement can have inertia (linear and angular)
    bool                        mTickMovement;
    // Linear
    float                       mMaxLinearAcceleration;
    vec2                        mLinearSteering;
    vec2                        mLinearVelocity;
    float                       mLinearInertiaSec;
    float                       mLinearInertiaCont;
    bool                        mLinearInertiaActivated;
    // Angular
    int8                        mTurnDirection; /* 1=left turn, -1=right turn*/
    float                       mMaxAngularAcceleration;
    float                       mAngularVelocity;
    float                       mAngularSteering;
    float                       mAngularInertiaSec;
    float                       mAngularInertiaCont;
    bool                        mAngularInertiaActivated;


    friend class Ptr<Entity>;
    friend class Ptr<const Entity>;
};

#endif