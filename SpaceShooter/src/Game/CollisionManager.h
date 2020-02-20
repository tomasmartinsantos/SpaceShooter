#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include "../Engine/Types.h"


class CCollision;

class CollisionManager
{
    // Methods
public:
    static Ptr<CollisionManager> Instance();

    void UpdateCollisions();
    bool DoesCollide(Ptr<CCollision> Component1, Ptr<CCollision> Component2);
    void CheckCollisionSceneLimits(Ptr<CCollision> Component);

    void AddCollisionComponent(Ptr<CCollision> Component);
    void RemoveCollisionComponent(Ptr<CCollision> Component);
    void RemoveAllCollisionComponents();

protected:
    CollisionManager();
    virtual ~CollisionManager();

    // Variables
private:
    static Ptr<CollisionManager> mInstance;

    std::vector<Ptr<CCollision>> mCollisionComponents;

    friend class Ptr<CollisionManager>;
    friend class Ptr<const CollisionManager>;
};
#endif