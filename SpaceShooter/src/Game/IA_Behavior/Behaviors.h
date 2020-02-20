#ifndef BEHAVIORS_H
#define BEHAVIORS_H

#include "../../Engine/Types.h"
#include "../../Engine/IA/BehaviourTree.h"

class CBrain;

/* BEHAVIORS */

// ATTACK
class Attack : public Behavior
{
    // Methods
public:
    static Ptr<Attack> Create(Ptr<CBrain> BrainComponent);
protected:
    Attack(Ptr<CBrain> OwnerBrain);
    virtual ~Attack();
    Status Update(float Step);

    // Variables
private:
    WPtr<CBrain> mOwnerBrain;

    friend class Ptr<Attack>;
    friend class Ptr<const Attack>;
};

// MOVEMENT
class WantToMove : public Behavior
{
    // Methods
public:
    static Ptr<WantToMove> Create(Ptr<CBrain> BrainComponent, float TargetReachedRadius);
protected:
    WantToMove(Ptr<CBrain> OwnerBrain, float TargetReachedRadius);
    virtual ~WantToMove();
    Status Update(float Step);

    // Variables
private:
    WPtr<CBrain> mOwnerBrain;
    float mTargetReachedRadius;

    friend class Ptr<WantToMove>;
    friend class Ptr<const WantToMove>;
};

class MoveToTarget : public Behavior
{
    // Methods
public:
    static Ptr<MoveToTarget> Create(Ptr<CBrain> BrainComponent, float TargetReachedRadius);
protected:
    MoveToTarget(Ptr<CBrain> OwnerBrain, float TargetReachedRadius);
    virtual ~MoveToTarget();
    Status Update(float Step);

    // Variables
private:
    WPtr<CBrain> mOwnerBrain;
    float mTargetReachedRadius;

    friend class Ptr<MoveToTarget>;
    friend class Ptr<const MoveToTarget>;
};

// TARGETING
class RotateToTarget : public Behavior
{
    // Methods
public:
    static Ptr<RotateToTarget> Create(Ptr<CBrain> BrainComponent, bool TargetingOpponent, float TargetReachedValue);
protected:
    RotateToTarget(Ptr<CBrain> OwnerBrain, bool TargetingOpponent, float TargetReachedValue);
    virtual ~RotateToTarget();
    Status Update(float Step);

    // Variables
private:
    WPtr<CBrain> mOwnerBrain;
    bool  mTargetingOpponent;
    float mTargetReachedValue;
    

    friend class Ptr<RotateToTarget>;
    friend class Ptr<const RotateToTarget>;
};

// IDLE
class Idle : public Behavior
{
    // Methods
public:
    static Ptr<Idle> Create(Ptr<CBrain> BrainComponent);
protected:
    Idle(Ptr<CBrain> OwnerBrain);
    virtual ~Idle();
    Status Update(float Step);

    // Variables
private:
    WPtr<CBrain> mOwnerBrain;

    friend class Ptr<Idle>;
    friend class Ptr<const Idle>;
};

#endif