#ifndef WORLD_H
#define WORLD_H


#include "../Engine/Types.h"

class Scene;
class Entity;

class World
{
    // Methods
public:
    static Ptr<World> Instance();

    void Main();
    Ptr<Scene> GetCurrentScene() const { return mCurrentScene; }
    void CheckScene();
    void ChangeScene();

    static bool IsEntityValid(Ptr<Entity> _Entity);

protected:
    World();
    virtual ~World();

private:
    void ProcessInput();
    void Update();
    void Render();
    void EndWorld();

    // Variables
private:
    static Ptr<World> mInstance;

    Ptr<Scene> mCurrentScene;

    friend class Ptr<World>;
    friend class Ptr<const World>;
};

#endif