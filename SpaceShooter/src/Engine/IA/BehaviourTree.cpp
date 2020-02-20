#include "BehaviourTree.h"

Ptr<Behavior> Behavior::Create()
{
    return new Behavior();
}

Status Behavior::Tick(float Step)
{
    if (mStatus == eInvalid)
        OnEnter();
    mContAbortBehaviorTimer += Step;
    mStatus = Update(Step);
    if (mStatus != eRunning)
        OnExit();
    return mStatus;
}

Ptr<Group> Group::Create()
{
    return new Group();
}

Ptr<Sequence> Sequence::Create()
{
    return new Sequence();
}

Status Sequence::Update(float Step)
{
    while (true)
    {
        if (mChildren[mCurrentChild] != nullptr)
        {
            Status CurrentStatus = mChildren[mCurrentChild]->Tick(Step);

            if (CurrentStatus != eSuccess)
                return CurrentStatus;
            ++mCurrentChild;
            if (mCurrentChild == mChildren.size())
                return eSuccess;
        }
    }
    return eInvalid;
}

Ptr<Selector> Selector::Create()
{
    return new Selector();
}

Status Selector::Update(float Step)
{
    while (true)
    {
        if (mChildren[mCurrentChild] != nullptr)
        {
            Status CurrentStatus = mChildren[mCurrentChild]->Tick(Step);

            if (CurrentStatus != eFail)
                return CurrentStatus;
            ++mCurrentChild;
            if (mCurrentChild == mChildren.size())
                return eFail;
        }
    }
    return eInvalid;
}

Ptr<BehaviorTree> BehaviorTree::Create()
{
    return new BehaviorTree();
}

BehaviorTree::BehaviorTree()
{

}

BehaviorTree::~BehaviorTree()
{
    mRoot = nullptr;
}

void BehaviorTree::Init()
{

}

void BehaviorTree::Update(float Step)
{
    if (mRoot != nullptr)
    {
        if (mRoot->GetStatus() == eSuccess)
        {
            mRoot->SetStatus(eInvalid);
        }
        mRoot->Tick(Step);
    }
}