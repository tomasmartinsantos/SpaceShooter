#ifndef BEHAVIOURTREE_H
#define BEHAVIOURTREE_H

#include "../Types.h"

enum Status
{
    eInvalid,
    eSuccess,
    eFail,
    eRunning
};

class Behavior
{
    // Methods
public:
    static Ptr<Behavior> Create();
    Status Tick(float Step);
    Status GetStatus() const { return mStatus; }
    void SetStatus(Status NewStatus) { mStatus = NewStatus; }
    float GetAbortTimer() const { return mAbortBehaviorTimer; }
    void SetAbortTimer(float Value) { mAbortBehaviorTimer = Value; }
protected:
    Behavior() { mStatus = eInvalid; mContAbortBehaviorTimer = 0.0f; mAbortBehaviorTimer = 60.0f; }
    virtual ~Behavior() {}

    virtual Status Update(float step) { return eInvalid; }
    virtual void OnEnter() { mContAbortBehaviorTimer = 0.0f; }
    virtual void OnExit() { mContAbortBehaviorTimer = 0.0f; }

    // Variables
protected:
    Status  mStatus;
    float   mAbortBehaviorTimer;
    float   mContAbortBehaviorTimer;

private:

    friend class Ptr<Behavior>;
    friend class Ptr<const Behavior>;
};

class Group : public Behavior
{
    // Methods
public:
    static Ptr<Group> Create();
    void AddBehavior(Ptr<Behavior> Behavior) { mChildren.push_back(Behavior); }
protected:
    Group() {}
    virtual ~Group() {}

    // Variables
protected:
    typedef std::vector<Ptr<Behavior>> Behaviors;
    Behaviors mChildren;
private:
    friend class Ptr<Group>;
    friend class Ptr<const Group>;
};

class Sequence : public Group
{
    // Methods
public:
    static Ptr<Sequence> Create();
protected:
    Sequence() { mCurrentChild = 0; mStatus = eInvalid; }
    virtual ~Sequence() {}

    void OnEnter() { Behavior::OnEnter(); mCurrentChild = 0; }
    Status Update(float Step);
    void OnExit() { Behavior::OnExit(); mCurrentChild = 0; }

    // Variables
protected:
    int mCurrentChild;
private:
    friend class Ptr<Sequence>;
    friend class Ptr<const Sequence>;
};

class Selector : public Group
{
    // Methods
public:
    static Ptr<Selector> Create();
protected:
    Selector() { mCurrentChild = 0; mStatus = eInvalid; }
    virtual ~Selector() {}

    void OnEnter() { Behavior::OnEnter(); mCurrentChild = 0; }
    Status Update(float Step);
    void OnExit() { Behavior::OnExit(); mCurrentChild = 0; }

    // Variables
protected:
    int mCurrentChild;
private:
    friend class Ptr<Selector>;
    friend class Ptr<const Selector>;
};

class BehaviorTree
{
    // Methods
public:
    static Ptr<BehaviorTree> Create();

    void Init();
    void Update(float Step);
    void SetRoot(Ptr<Group> Root) { mRoot = Root; }

protected:
    BehaviorTree();
    virtual ~BehaviorTree();
    // Variables
private:
    Ptr<Group> mRoot;

    friend class Ptr<BehaviorTree>;
    friend class Ptr<const BehaviorTree>;
};

#endif