#include "Components/CCollision.h"
#include "Entities/HeadersEntities.h"
#include "../Engine/Math.h"
#include "World.h"
#include "Scene.h"
#include "UI/WLifebar.h"
#include "Components/CWeapon.h"
#include "Components/CAnimation.h"
#include "../Engine/Audio/Audiosource.h"
#include "CollisionManager.h"


Ptr<CollisionManager> CollisionManager::mInstance = nullptr;

Ptr<CollisionManager> CollisionManager::Instance()
{
    if (mInstance == nullptr)
        mInstance = new CollisionManager();
    return mInstance;
}

CollisionManager::CollisionManager()
{

}

CollisionManager::~CollisionManager()
{
    RemoveAllCollisionComponents();
}

void CollisionManager::UpdateCollisions()
{
    // Go through all Collision Components
    for (std::vector<Ptr<CCollision>>::iterator itMain = mCollisionComponents.begin(); itMain != mCollisionComponents.end(); ++itMain)
    {
        if ((*itMain) != nullptr && (*itMain)->GetTickCollision()) // If Collision is activated 
        {
            // First, check Scene Limits
            CheckCollisionSceneLimits((*itMain));
            for (std::vector<Ptr<CCollision>>::iterator itSecond = itMain; itSecond != mCollisionComponents.end(); ++itSecond)
            {
                if ((*itSecond) != nullptr && (*itMain) != (*itSecond) && (*itSecond)->GetTickCollision() && World::IsEntityValid((*itMain)->GetMyEntity()) && World::IsEntityValid((*itSecond)->GetMyEntity()) && DoesCollide((*itMain), (*itSecond)))
                {
                    // If both Components collide, then manage this collision
                    (*itMain)->ManageCollision((*itSecond));
                    (*itSecond)->ManageCollision((*itMain));
                }
            }
        }
    }
}

bool CollisionManager::DoesCollide(Ptr<CCollision> Component1, Ptr<CCollision> Component2)
{
    // Check collision (circle collision type)
    if(Component1 != nullptr && Component2!=nullptr && World::IsEntityValid(Component1->GetMyEntity()) && World::IsEntityValid(Component2->GetMyEntity()))
        return (SqrDistance(Component1->GetMyEntity()->GetPosition(), Component2->GetMyEntity()->GetPosition()) <= powf(Component1->GetCollisionRadius() + Component2->GetCollisionRadius(), 2));
    return false;
}

void CollisionManager::CheckCollisionSceneLimits(Ptr<CCollision> Component)
{
    Ptr<Scene> CurrentScene = World::Instance()->GetCurrentScene();
    if (Component != nullptr && CurrentScene != nullptr)
    {
        if (World::IsEntityValid(Component->GetMyEntity()))
        {
            vec2 Position = Component->GetMyEntity()->GetPosition();
            vec2 NewPos = Position;

            // If Entity is a Ship, reallocate it inside Scene limits
            if (Component->GetMyEntity().DownCast<EShip>() != nullptr)
            {
                if (Position.x > CurrentScene->GetSceneLimitMaxX())
                    NewPos.x = (float)CurrentScene->GetSceneLimitMaxX();
                if (Position.x < CurrentScene->GetSceneLimitMinX())
                    NewPos.x = (float)CurrentScene->GetSceneLimitMinX();

                if (Position.y > CurrentScene->GetSceneLimitMaxY())
                    NewPos.y = (float)CurrentScene->GetSceneLimitMaxY();
                if (Position.y < CurrentScene->GetSceneLimitMinY())
                    NewPos.y = (float)CurrentScene->GetSceneLimitMinY();

                Component->GetMyEntity()->SetPosition(NewPos);
            }

            // If Entity is a Projectile, destroy it
            else if (Component->GetMyEntity().DownCast<EProjectile>() != nullptr)
            {
                if (Position.x > CurrentScene->GetSceneLimitMaxX() || Position.x < CurrentScene->GetSceneLimitMinX() || Position.y > CurrentScene->GetSceneLimitMaxY() || Position.y < CurrentScene->GetSceneLimitMinY())
                    Component->GetMyEntity()->Destroy();
            }
        }
    }
}

void CollisionManager::AddCollisionComponent(Ptr<CCollision> Component)
{
    if (Component != nullptr)
    {
        mCollisionComponents.push_back(Component);
    }
}

void CollisionManager::RemoveCollisionComponent(Ptr<CCollision> Component)
{
    for (std::vector<Ptr<CCollision>>::iterator it = mCollisionComponents.begin(); it != mCollisionComponents.end();)
    {
        if ((*it) == Component)
            it = mCollisionComponents.erase(it);
        else
            ++it;
    }
}

void CollisionManager::RemoveAllCollisionComponents()
{
    size_t Size = mCollisionComponents.size();
    for(uint32 i = 0; i < Size; ++i)
    {
        mCollisionComponents[i] = nullptr;
    }
    mCollisionComponents.clear();
}