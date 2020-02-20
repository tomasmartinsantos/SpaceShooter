#ifndef SCENE_H
#define SCENE_H

#include "../Engine/Types.h"
#include "../Engine/String.h"
#include "Entities/Entity.h"
#include "Entities/EProjectile.h"

class Image;
class EProjectile;
class EShip;
class Widget;
class Font;

class Scene
{
    // Methods
public:
    enum SceneState
    {
        START_STATE,
        INGAME_STATE,
        FINISH_1P_WIN_STATE,
        FINISH_1P_LOSE_STATE,
        CHANGING_SCENE
    };
    static Ptr<Scene> Create();

    virtual void Init(const String& Filename);
    virtual void ProcessInput();
    virtual void Update(float elapsed);
    virtual void Render();
    virtual void EndScene();

    void        GoToState(SceneState NextState);
    SceneState  GetSceneState() const { return mCurrentState; }


    virtual Ptr<Entity> SpawnEntity(Entity::EntityType TypeToSpawn, const Transform2D& Transform, float MaxLinearAcceleration = 0.0f, float MaxAngularAcceleration = 0.0f, float LinearInertiaSec = 0.0f, float AngularInertiaSec = 0.0f);
    virtual Ptr<Entity> CopyEntity(Ptr<Entity> EntityToCopy);

    virtual void DestroyPendingKillEntities();
    virtual void DestroyAllEntities();

    virtual void AddHUDToScene(Ptr<Widget> HUD);

    int                 GetSceneLimitMinX() const { return mSceneLimitMinX; }
    int                 GetSceneLimitMaxX() const { return mSceneLimitMaxX; }
    int                 GetSceneLimitMinY() const { return mSceneLimitMinY; }
    int                 GetSceneLimitMaxY() const { return mSceneLimitMaxY; }

    void                ReadEntitiesXML(const String& Filename);
    void                ReadProjectileCollectionXML(const String& Filename);
    void                ReadHUDXML(const String& Filename);
    void                ReadBackgroundXML(const String& Filename);

    int                 GetProjectileCollectionIndex(EProjectile::ProjectileType Type) const;
    Ptr<EProjectile>    GetProjectileInCollection(int index);

    Ptr<EShip>         GetOpponent(Ptr<EShip> Ship); // Return the opponent of the given Ship

protected:
    Scene();
    virtual ~Scene();

    // Variables
private:
    std::vector<Ptr<Entity>>        mEntities;
    Ptr<Widget>                     mHUD;

    //Scene current state
    SceneState                      mCurrentState;

    // Scene Limits
    int                             mSceneLimitMinX;
    int                             mSceneLimitMaxX;
    int                             mSceneLimitMinY;
    int                             mSceneLimitMaxY;

    std::vector<Ptr<EProjectile>>   mProjectileCollection;

    // Font and Text
    Ptr<Font>                       mFont;
    String                          mGetReadyText;
    String                          mWinText;
    String                          mLoseText;
    float                           mContTimeDisplayText;
    float                           mTimeStartGameText;
    float                           mTimeFinishGameText;


    friend class Ptr<Scene>;
    friend class Ptr<const Scene>;
};

#endif
