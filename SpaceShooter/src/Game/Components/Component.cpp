#include "../Entities/Entity.h"
#include "CAnimation.h"
#include "../../Engine/Audio/Audiosource.h"
#include "Component.h"


Ptr<Component> Component::Create(WPtr<Entity> _Entity)
{
    if (_Entity != nullptr)
    {
        Ptr<Component> p = new Component(_Entity);
        p->mThis = p;
        return p;
    }
    else
        return nullptr;
}

Component::Component()
{
    mType   = ComponentType::COMPONENT_BASIC;
    mThis   = nullptr;
    mEntity = nullptr;
    mAudio  = nullptr;
}

Component::Component(WPtr<Entity> _Entity)
{
    mType   = ComponentType::COMPONENT_BASIC;
    mThis   = nullptr;
    mEntity = _Entity;
    mAudio  = nullptr;
}

Component::~Component()
{
    mThis   = nullptr;
    mEntity = nullptr;
    mAudio  = nullptr;
}

void Component::Init()
{

}

void Component::Update(float elapsed)
{

}