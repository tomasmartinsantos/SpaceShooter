#ifndef COMPONENT_H
#define COMPONENT_H

#include "../../Engine/Types.h"

class Entity;
class AudioSource;

class Component
{
    // Methods
public:
    enum ComponentType
    {
        COMPONENT_BASIC,
        COMPONENT_COLLISION,
        COMPONENT_ANIMATION,
        COMPONENT_WEAPON,
        COMPONENT_BRAIN
    };
    static Ptr<Component> Create(WPtr<Entity> _Entity);

    virtual void Init();
    virtual void Update(float elapsed);

    ComponentType GetType() const { return mType; }

    Ptr<AudioSource> GetAudio() const                   { return mAudio; }
    void             SetAudio(Ptr<AudioSource> Audio)   { mAudio = Audio; }

protected:
    Component();
    Component(WPtr<Entity> _Entity);
    virtual ~Component();

    // Variables
protected:
    WPtr<Component> mThis;
    WPtr<Entity>    mEntity;
    ComponentType   mType;

    Ptr<AudioSource> mAudio;

private:
    friend class Ptr<Component>;
    friend class Ptr<const Component>;
};

#endif