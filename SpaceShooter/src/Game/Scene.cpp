#include "World.h"
#include "../Engine/Resourcemanager.h"
#include "../Engine/Screen.h"
#include "../Engine/Image.h"
#include "../../vendor/rapidxml/rapidxml.hpp"
#include "../Engine/Input/Controller.h"
#include "../Engine/Input/InputManager.h"
#include "../Engine/Input/InputEvent.h"
#include "../Engine/Renderer.h"
#include "CollisionManager.h"
#include "Components/HeadersComponents.h"
#include "Entities/HeadersEntities.h"
#include "UI/HeadersUI.h"
#include "../Engine/Audio/Audiobuffer.h"
#include "../Engine/Audio/Audiosource.h"
#include "../Engine/Font.h"
#include "Scene.h"


using namespace rapidxml;

Ptr<Scene> Scene::Create()
{
    return new Scene();
}

Scene::Scene()
{
    mHUD            = nullptr;
    mFont           = nullptr;
    mCurrentState   = SceneState::START_STATE;
}

Scene::~Scene()
{
    EndScene();
}

void Scene::Init(const String& Filename)
{
    // State
    mCurrentState           = SceneState::START_STATE;

    // Scene Limits
    mSceneLimitMinX         = 0;
    mSceneLimitMaxX         = Screen::Instance()->GetWidth() - HUD_SIZE;
    mSceneLimitMinY         = 0;
    mSceneLimitMaxY         = Screen::Instance()->GetHeight();

    //Font and Texts
    mFont                   = ResourceManager::Instance()->LoadFont("data/Fonts/monospaced.png");
    mGetReadyText           = "GET READY";
    mWinText                = "YOU WIN";
    mLoseText               = "YOU LOSE";
    mContTimeDisplayText    = 0.0f;
    mTimeStartGameText      = 3.0f;
    mTimeFinishGameText     = 3.0f;

    // Read all the Scene info from a XML file
    String Buffer = String::Read(Filename);
    if (Buffer != "")
    {
        // Read XML
        xml_document<> Doc;
        Doc.parse<0>((char*)Buffer.ToCString());
        xml_node<>* RootNode = Doc.first_node();

        if (RootNode != nullptr)
        {
            // Go through all XML documents to set up the scene

            // Background images
            xml_node<>* BackgroundNode = RootNode->first_node("Background");
            if (BackgroundNode != nullptr)
            {
                String Path = BackgroundNode->first_attribute("path")->value();
                ReadBackgroundXML(Path);
            }
            // Types of projectiles
            xml_node<>* ProjectilesCollectionNode = RootNode->first_node("ProjectilesCollection");
            if (ProjectilesCollectionNode != nullptr)
            {
                String Path = ProjectilesCollectionNode->first_attribute("path")->value();
                ReadProjectileCollectionXML(Path);
            }
            // All the entities that participate on the game
            xml_node<>* EntitiesNode = RootNode->first_node("Entities");
            if (EntitiesNode != nullptr)
            {
                String Path = EntitiesNode->first_attribute("path")->value();
                ReadEntitiesXML(Path);
            }
            // Player HUD
            xml_node<>* HUDNode = RootNode->first_node("HUD");
            if (HUDNode != nullptr)
            {
                String Path = HUDNode->first_attribute("path")->value();
                ReadHUDXML(Path);
            }
        }
    }
}

void Scene::ProcessInput()
{
    if(mCurrentState == SceneState::INGAME_STATE)
        IInputManagerController::Instance()->ProcessInput();
}

void Scene::Update(float Elapsed)
{
    // Only update entities and HUD if not changing from one scene to another
    if (mCurrentState != SceneState::CHANGING_SCENE)
    {
        if (mCurrentState == SceneState::START_STATE)
        {
            mContTimeDisplayText += Elapsed;
            if (mContTimeDisplayText >= mTimeStartGameText)
                GoToState(SceneState::INGAME_STATE);
        }

        else if (mCurrentState == SceneState::INGAME_STATE)
        {
            size_t Size = mEntities.size();
            for (uint32 i = 0; i < Size; ++i)
            {
                if (World::IsEntityValid(mEntities[i]))
                    mEntities[i]->Update(Elapsed);
            }
            CollisionManager::Instance()->UpdateCollisions();
            DestroyPendingKillEntities();
            if (mHUD != nullptr)
                mHUD->Update(Elapsed);
        }

        else if (mCurrentState == SceneState::FINISH_1P_WIN_STATE || mCurrentState == SceneState::FINISH_1P_LOSE_STATE)
        {
            mContTimeDisplayText += Elapsed;
            if (mContTimeDisplayText >= mTimeFinishGameText)
                GoToState(SceneState::CHANGING_SCENE);
        }
    }
}

void Scene::Render()
{
    // Only render entities and HUD if not changing from one scene to another
    if (mCurrentState != SceneState::CHANGING_SCENE)
    {
        size_t Size = mEntities.size();
        for (uint32 i = 0; i < Size; ++i)
        {
            if (World::IsEntityValid(mEntities[i]))
                mEntities[i]->Render();
        }
        if (mHUD != nullptr)
            mHUD->Render();

        if (mCurrentState == SceneState::START_STATE)
        {
            Renderer::Instance()->SetColor(255, 0, 0, 255);
            Renderer::Instance()->DrawText(mFont, mGetReadyText, 260, 300);
        }
        else if (mCurrentState == SceneState::FINISH_1P_WIN_STATE)
        {
            Renderer::Instance()->SetColor(255, 0, 0, 255);
            Renderer::Instance()->DrawText(mFont, mWinText, 260, 300);
        }
        else if (mCurrentState == SceneState::FINISH_1P_LOSE_STATE)
        {
            Renderer::Instance()->SetColor(255, 0, 0, 255);
            Renderer::Instance()->DrawText(mFont, mLoseText, 260, 300);
        }
    }
}

void Scene::EndScene()
{
    // When ending a Scene, all CollisionComponents, InputManagerObservers and Entities should be removed, plus HUD and Fonts
    CollisionManager::Instance()->RemoveAllCollisionComponents();
    IInputManagerObserver::Instance()->RemoveAllObservers();
    DestroyAllEntities();
    mHUD = nullptr;
    mFont = nullptr;
}

void Scene::GoToState(SceneState NextState)
{
    // Trandition to New State
    mContTimeDisplayText = 0.0;
    mCurrentState = NextState;
}

Ptr<Entity> Scene::SpawnEntity(Entity::EntityType TypeToSpawn, const Transform2D& Transform, float MaxLinearAcceleration, float MaxAngularAcceleration, float LinearInertiaSec, float AngularInertiaSec)
{
    // Spawn a new Entity on Scene, depending on the Type required
    Ptr<Entity> NewEntity;
    switch (TypeToSpawn)
    {
    case Entity::ENTITY_BASIC:
    {
        NewEntity = Entity::Create(Transform, MaxLinearAcceleration, MaxAngularAcceleration, LinearInertiaSec, AngularInertiaSec);
        break;
    }
    case Entity::ENTITY_SHIP:
    {
        NewEntity = (EShip::Create(Transform, MaxLinearAcceleration, MaxAngularAcceleration, LinearInertiaSec, AngularInertiaSec)).UpCast<Entity>();
        break;
    }
    case Entity::ENTITY_SHIPCONTROLLABLE:
    {
        NewEntity = (EShipControllable::Create(Transform, MaxLinearAcceleration, MaxAngularAcceleration, LinearInertiaSec, AngularInertiaSec)).UpCast<Entity>();
        break;
    }
    case Entity::ENTITY_PROJECTILE:
    {
        NewEntity = (EProjectile::Create(Transform, MaxLinearAcceleration, MaxAngularAcceleration, LinearInertiaSec, AngularInertiaSec)).UpCast<Entity>();
        break;
    }
    case Entity::ENTITY_ASTEROID:
    {
        NewEntity = (EAsteroid::Create(Transform, MaxLinearAcceleration, MaxAngularAcceleration, LinearInertiaSec, AngularInertiaSec)).UpCast<Entity>();
        break;
    }
    default:
        break;
    }
    if (World::IsEntityValid(NewEntity))
        mEntities.push_back(NewEntity);

    return NewEntity;
}

Ptr<Entity> Scene::CopyEntity(Ptr<Entity> EntityToCopy)
{
    // Create a new Entity based on the same parameters and components of an Entity given
    Ptr<Entity> NewCopiedEntity;
    if (World::IsEntityValid(EntityToCopy))
    {
        NewCopiedEntity = SpawnEntity(EntityToCopy->GetType(), EntityToCopy->GetTransform(), EntityToCopy->GetMaxLinearAcc(), EntityToCopy->GetMaxAngularAcc(), EntityToCopy->GetLinearInertia(), EntityToCopy->GetAngularInertia());
        if (World::IsEntityValid(NewCopiedEntity))
        {
            size_t Size = EntityToCopy->GetAllComponents().size();
            for (uint32 i = 0; i < Size; ++i)
            {
                if (EntityToCopy->GetAllComponents()[i] != nullptr)
                {
                    switch (EntityToCopy->GetAllComponents()[i]->GetType())
                    {
                    case Component::COMPONENT_BASIC:
                    {
                        NewCopiedEntity->AddComponent(Component::Create(NewCopiedEntity));
                        break;
                    }
                    case Component::COMPONENT_COLLISION:
                    {
                        NewCopiedEntity->AddComponent((CCollision::Create(NewCopiedEntity, EntityToCopy->GetAllComponents()[i].DownCast<CCollision>()->GetCollisionRadius(), EntityToCopy->GetAllComponents()[i].DownCast<CCollision>()->GetCollisionDamage(), EntityToCopy->GetAllComponents()[i]->GetAudio()).UpCast<Component>()));
                        break;
                    }
                    case Component::COMPONENT_ANIMATION:
                    {
                        NewCopiedEntity->AddComponent((CAnimation::Create(NewCopiedEntity, EntityToCopy->GetAllComponents()[i].DownCast<CAnimation>()->GetAllAnimations()).UpCast<Component>()));
                        break;
                    }
                    case Component::COMPONENT_WEAPON:
                    {
                        NewCopiedEntity->AddComponent((CWeapon::Create(NewCopiedEntity, EntityToCopy->GetAllComponents()[i].DownCast<CWeapon>()->GetProjectileCollectionIndex(), EntityToCopy->GetAllComponents()[i].DownCast<CWeapon>()->GetCooldownSecs(), EntityToCopy->GetAllComponents()[i]->GetAudio()).UpCast<Component>()));
                        break;
                    }
                    default:
                        break;
                    }
                }
            }
            NewCopiedEntity->Init();
        }
    }
    return NewCopiedEntity;
}

void Scene::DestroyPendingKillEntities()
{
    // After every Update, destroy all pending kill entities
    for (std::vector<Ptr<Entity>>::iterator itEntities = mEntities.begin(); itEntities != mEntities.end();)
    {
        if ((*itEntities) != nullptr && (*itEntities)->IsPendingKill())
        {
            if ((*itEntities).DownCast<EShipControllable>() != nullptr && ((*itEntities).DownCast<EShipControllable>()).UpCast<IIMObserver>() != nullptr)
                IInputManagerObserver::Instance()->RemoveObserver(((*itEntities).DownCast<EShipControllable>()).UpCast<IIMObserver>());
            itEntities = mEntities.erase(itEntities);
        }
        else
            ++itEntities;
    }
}

void Scene::AddHUDToScene(Ptr<Widget> HUD)
{
    if (HUD != nullptr)
        mHUD = HUD;
}

void Scene::DestroyAllEntities()
{
    // Destroy all entities
    size_t Size = mProjectileCollection.size();
    for(uint32 i = 0; i < Size; ++i)
    {
        mProjectileCollection[i] = nullptr;
    }
    mProjectileCollection.clear();

    Size = mEntities.size();
    for(uint32 i = 0; i < Size; ++i)
    {
        mEntities[i] = nullptr;
    }
    mEntities.clear();
}

void Scene::ReadEntitiesXML(const String& Filename)
{
    // Read the XML file that contains the info of all the Entities that should be on Scene
    String Buffer = String::Read(Filename);
    if (Buffer != "")
    {
        // Read XML
        xml_document<> Doc;
        Doc.parse<0>((char*)Buffer.ToCString());
        xml_node<>* RootNode = Doc.first_node();

        if (RootNode != nullptr)
        {

            // Go through all entities
            Ptr<Entity> NewEntity;
            xml_node<>* EntityNode = RootNode->first_node("Entity");
            while (EntityNode != nullptr)
            {
                String Type = EntityNode->first_attribute("type")->value();
                vec2 Position = vec2();
                float Rotation = 0.0f;
                vec2 Scale = vec2(1.0f, 1.0f);
                float MaxLinearAcc = 0.0f;
                float MaxAngularAcc = 0.0f;
                float LinearInertia = 0.0f;
                float AngularInertia = 0.0f;

                // Transform
                xml_node<>* TransformNode = EntityNode->first_node("Transform");
                if (TransformNode != nullptr)
                {
                    xml_node<>* PositionNode = TransformNode->first_node("Position");
                    xml_node<>* RotationNode = TransformNode->first_node("Rotation");
                    xml_node<>* ScaleNode = TransformNode->first_node("Scale");
                    if (TransformNode != nullptr && PositionNode != nullptr && ScaleNode != nullptr)
                    {
                        Position = vec2((float)(atof(PositionNode->first_attribute("x")->value())), (float)(atof(PositionNode->first_attribute("y")->value())));
                        Rotation = (float)(atof(RotationNode->first_attribute("value")->value()));
                        Scale = vec2((float)(atof(ScaleNode->first_attribute("x")->value())), (float)(atof(ScaleNode->first_attribute("y")->value())));
                    }
                }

                // Movement
                xml_node<>* MovementNode = EntityNode->first_node("Movement");
                if (MovementNode != nullptr)
                {
                    xml_node<>* LinearNode = MovementNode->first_node("MaxLinearAcceleration");
                    xml_node<>* AngularNode = MovementNode->first_node("MaxAngularAcceleration");
                    xml_node<>* LinearInertiaNode = MovementNode->first_node("LinearInertia");
                    xml_node<>* AngularInertiaNode = MovementNode->first_node("AngularInertia");
                    if (LinearNode != nullptr && AngularNode != nullptr && LinearInertiaNode != nullptr && AngularInertiaNode != nullptr)
                    {
                        MaxLinearAcc = (float)(atof(LinearNode->first_attribute("value")->value()));
                        MaxAngularAcc = (float)(atof(AngularNode->first_attribute("value")->value()));
                        LinearInertia = (float)(atof(LinearInertiaNode->first_attribute("value")->value()));
                        AngularInertia = (float)(atof(AngularInertiaNode->first_attribute("value")->value()));
                    }
                }

                // Create the entity
                NewEntity = SpawnEntity(Entity::GetEntTypeFromString(Type), Transform2D(Position, Rotation, Scale), MaxLinearAcc, MaxAngularAcc, LinearInertia, AngularInertia);

                if (World::IsEntityValid(NewEntity))
                {
                    // Components
                    xml_node<>* ComponentNode = EntityNode->first_node("Component");
                    while (ComponentNode != nullptr)
                    {
                        String Type = ComponentNode->first_attribute("type")->value();
                        if (Type == "COMPONENT_BRAIN")
                        {
                            float SecondsToNewTarget = 0.0f;
                            float RotatingReachedValue = 0.0f;
                            float MovementReachedValue = 0.0f;

                            xml_node<>* SecondsNode = ComponentNode->first_node("SecondsToNewTarget");
                            if (SecondsNode != nullptr)
                                SecondsToNewTarget = (float)(atof(SecondsNode->first_attribute("value")->value()));
                            xml_node<>* RotatingReachedNode = ComponentNode->first_node("RotatingReachedValue");
                            if (RotatingReachedNode != nullptr)
                                RotatingReachedValue = (float)(atof(RotatingReachedNode->first_attribute("value")->value()));
                            xml_node<>* MovementReachedNode = ComponentNode->first_node("MovementReachedValue");
                            if (MovementReachedNode != nullptr)
                                MovementReachedValue = (float)(atof(MovementReachedNode->first_attribute("value")->value()));
                            NewEntity->AddComponent((CBrain::Create(NewEntity, SecondsToNewTarget, RotatingReachedValue, MovementReachedValue).UpCast<Component>()));
                        }
                        else if (Type == "COMPONENT_COLLISION")
                        {
                            float CollisionRadius = 0.0f;
                            int Damage = 0;

                            xml_node<>* RadiusNode = ComponentNode->first_node("CollisionRadius");
                            if (RadiusNode != nullptr)
                            {
                                CollisionRadius = (float)(atof(RadiusNode->first_attribute("value")->value()));
                                float ScaleRate = (Scale.x + Scale.y) / 2.0f;
                                CollisionRadius *= ScaleRate;
                            }
                            xml_node<>* DamageNode = ComponentNode->first_node("Damage");
                            if (DamageNode != nullptr)
                            {
                                Damage = atoi(DamageNode->first_attribute("value")->value());
                            }
                            xml_node<>* AudioNode = ComponentNode->first_node("Sound");
                            String Path = "";
                            if (AudioNode != nullptr)
                            {
                                Path = AudioNode->first_attribute("path")->value();
                            }
                            Ptr<AudioBuffer> ExplosionSound = ResourceManager::Instance()->LoadAudio(Path);
                            Ptr<AudioSource> ExplosionSoundSource;
                            if (ExplosionSound != nullptr && ExplosionSound->IsValid())
                                ExplosionSoundSource = AudioSource::Create(ExplosionSound);
                            NewEntity->AddComponent((CCollision::Create(NewEntity, CollisionRadius, Damage, ExplosionSoundSource).UpCast<Component>()));
                        }
                        else if (Type == "COMPONENT_ANIMATION")
                        {
                            xml_node<>* AnimationNode = ComponentNode->first_node("Animation");
                            std::vector<std::pair<int, std::vector<Ptr<Image>>>> Animations;
                            while (AnimationNode != nullptr)
                            {

                                xml_node<>* AnimFPSNode = AnimationNode->first_node("AnimFPS");
                                int AnimFPS = 1;
                                if (AnimFPSNode != nullptr)
                                    AnimFPS = atoi(AnimFPSNode->first_attribute("value")->value());

                                xml_node<>* FixImageRotationNode = AnimationNode->first_node("FixImageRotation");
                                float FixAngle = 0.0f;
                                if (FixImageRotationNode != nullptr)
                                    FixAngle = (float)(atof(FixImageRotationNode->first_attribute("value")->value()));

                                xml_node<>* ImageNode = AnimationNode->first_node("Image");
                                std::vector<Ptr<Image>> AnimImages;
                                while (ImageNode != nullptr)
                                {
                                    Ptr<Image> NewImage = ResourceManager::Instance()->LoadImage(ImageNode->first_attribute("path")->value(), FixAngle);
                                    AnimImages.push_back(NewImage);
                                    ImageNode = ImageNode->next_sibling("Image");
                                }
                                std::pair<int, std::vector<Ptr<Image>>> AnimationPair;
                                AnimationPair.first = AnimFPS;
                                AnimationPair.second = AnimImages;
                                Animations.push_back(AnimationPair);
                                AnimationNode = AnimationNode->next_sibling("Animation");
                            }

                            NewEntity->AddComponent((CAnimation::Create(NewEntity, Animations).UpCast<Component>()));
                        }
                        else if (Type == "COMPONENT_WEAPON")
                        {
                            xml_node<>* SpawnPosNode = ComponentNode->first_node("RelativeSpawnPos");
                            vec2 SpawnPos = vec2();
                            if (SpawnPosNode != nullptr)
                                SpawnPos = vec2((float)(atof(SpawnPosNode->first_attribute("x")->value())), (float)(atof(SpawnPosNode->first_attribute("y")->value())));


                            xml_node<>* CooldownNode = ComponentNode->first_node("CooldownSecs");
                            float Cooldown = 0.0f;
                            if (CooldownNode != nullptr)
                                Cooldown = (float)(atof(CooldownNode->first_attribute("value")->value()));

                            xml_node<>* ProjectilePrototypeNode = ComponentNode->first_node("ProjectilePrototype");
                            EProjectile::ProjectileType ProjectileType;
                            if (ProjectilePrototypeNode != nullptr)
                                ProjectileType = EProjectile::GetProjectileTypeFromString(ProjectilePrototypeNode->first_attribute("type")->value());

                            xml_node<>* AudioNode = ComponentNode->first_node("Sound");
                            String Path = "";
                            if (AudioNode != nullptr)
                            {
                                Path = AudioNode->first_attribute("path")->value();
                            }
                            // Audio
                            Ptr<AudioBuffer> FireSound = ResourceManager::Instance()->LoadAudio(Path);
                            Ptr<AudioSource> FireSoundSource;
                            if (FireSound != nullptr && FireSound->IsValid())
                                FireSoundSource = AudioSource::Create(FireSound);

                            NewEntity->AddComponent((CWeapon::Create(NewEntity, GetProjectileCollectionIndex(ProjectileType), Cooldown, FireSoundSource).UpCast<Component>()));
                            if (NewEntity.DownCast<EShip>() != nullptr)
                                NewEntity.DownCast<EShip>()->SetRelativeProjectileSpawnPos(SpawnPos);
                        }
                        ComponentNode = ComponentNode->next_sibling("Component");
                    }

                    // If the Entity is a Ship, there is a LifeNode on the XML
                    if (NewEntity.DownCast<EShip>() != nullptr)
                    {
                        xml_node<>* LifeNode = EntityNode->first_node("Life");
                        if (LifeNode != nullptr)
                        {
                            int Life = atoi(LifeNode->first_attribute("value")->value());
                            NewEntity.DownCast<EShip>()->SetLife(Life);
                        }
                    }

                    // Initialize Entity and its components
                    NewEntity->Init();

                    EntityNode = EntityNode->next_sibling("Entity");
                }
            }
        }
    }
}

void Scene::ReadProjectileCollectionXML(const String& Filename)
{
    // Read the XML file that contains the info of the different types of projectiles that Ships will fire
    Ptr<EProjectile> NewProjectile;
    String Buffer = String::Read(Filename);
    if (Buffer != "")
    {
        // Read XML
        xml_document<> Doc;
        Doc.parse<0>((char*)Buffer.ToCString());
        xml_node<>* RootNode = Doc.first_node();

        if (RootNode != nullptr)
        {
            // Go through all projectiles
            xml_node<>* EntityNode = RootNode->first_node("Entity");
            while (EntityNode != nullptr)
            {
                String Type = EntityNode->first_attribute("type")->value();
                if (Type == "ENTITY_PROJECTILE")
                {
                    vec2 Position = vec2();
                    float Rotation = 0.0f;
                    vec2 Scale = vec2(1.0f, 1.0f);
                    float MaxLinearAcc = 0.0f;
                    float MaxAngularAcc = 0.0f;
                    float LinearInertia = 0.0f;
                    float AngularInertia = 0.0f;
                    EProjectile::ProjectileType ProjectileType;

                    // ProjectileType
                    xml_node<>* ProjectileTypeNode = EntityNode->first_node("ProjectileType");
                    if (ProjectileTypeNode != nullptr)
                        ProjectileType = EProjectile::GetProjectileTypeFromString(ProjectileTypeNode->first_attribute("type")->value());

                    // Transform
                    xml_node<>* TransformNode = EntityNode->first_node("Transform");
                    if (TransformNode != nullptr)
                    {
                        xml_node<>* PositionNode = TransformNode->first_node("Position");
                        xml_node<>* RotationNode = TransformNode->first_node("Rotation");
                        xml_node<>* ScaleNode = TransformNode->first_node("Scale");
                        if (TransformNode != nullptr && PositionNode != nullptr && ScaleNode != nullptr)
                        {
                            Position = vec2((float)(atof(PositionNode->first_attribute("x")->value())), (float)(atof(PositionNode->first_attribute("y")->value())));
                            Rotation = (float)(atof(RotationNode->first_attribute("value")->value()));
                            Scale = vec2((float)(atof(ScaleNode->first_attribute("x")->value())), (float)(atof(ScaleNode->first_attribute("y")->value())));
                        }
                    }

                    // Movement
                    xml_node<>* MovementNode = EntityNode->first_node("Movement");
                    if (MovementNode != nullptr)
                    {
                        xml_node<>* LinearNode = MovementNode->first_node("MaxLinearAcceleration");
                        xml_node<>* AngularNode = MovementNode->first_node("MaxAngularAcceleration");
                        xml_node<>* LinearInertiaNode = MovementNode->first_node("LinearInertia");
                        xml_node<>* AngularInertiaNode = MovementNode->first_node("AngularInertia");
                        if (LinearNode != nullptr && AngularNode != nullptr && LinearInertiaNode != nullptr && AngularInertiaNode != nullptr)
                        {
                            MaxLinearAcc = (float)(atof(LinearNode->first_attribute("value")->value()));
                            MaxAngularAcc = (float)(atof(AngularNode->first_attribute("value")->value()));
                            LinearInertia = (float)(atof(LinearInertiaNode->first_attribute("value")->value()));
                            AngularInertia = (float)(atof(AngularInertiaNode->first_attribute("value")->value()));
                        }
                    }

                    // Create the entity
                    NewProjectile = EProjectile::Create(Transform2D(Position, Rotation, Scale), MaxLinearAcc, MaxAngularAcc, LinearInertia, AngularInertia);

                    if (World::IsEntityValid(NewProjectile.UpCast<Entity>()))
                    {
                        NewProjectile->SetProjectileType(ProjectileType);
                        mProjectileCollection.push_back(NewProjectile);

                        // Components
                        xml_node<>* ComponentNode = EntityNode->first_node("Component");
                        while (ComponentNode != nullptr)
                        {
                            String Type = ComponentNode->first_attribute("type")->value();

                            if (Type == "COMPONENT_COLLISION")
                            {
                                float CollisionRadius = 0.0f;
                                int Damage = 0;

                                xml_node<>* RadiusNode = ComponentNode->first_node("CollisionRadius");
                                if (RadiusNode != nullptr)
                                {
                                    CollisionRadius = (float)(atof(RadiusNode->first_attribute("value")->value()));
                                    float ScaleRate = (Scale.x + Scale.y) / 2.0f;
                                    CollisionRadius *= ScaleRate;
                                }
                                xml_node<>* DamageNode = ComponentNode->first_node("Damage");
                                if (DamageNode != nullptr)
                                {
                                    Damage = atoi(DamageNode->first_attribute("value")->value());
                                }
                                xml_node<>* AudioNode = ComponentNode->first_node("Sound");
                                String Path = "";
                                if (AudioNode != nullptr)
                                {
                                    Path = AudioNode->first_attribute("path")->value();
                                }
                                // Audio
                                Ptr<AudioBuffer> ExplosionSound = ResourceManager::Instance()->LoadAudio(Path);
                                Ptr<AudioSource> ExplosionSoundSource;
                                if (ExplosionSound != nullptr && ExplosionSound->IsValid())
                                    ExplosionSoundSource = AudioSource::Create(ExplosionSound);

                                NewProjectile->AddComponent((CCollision::Create(NewProjectile.UpCast<Entity>(), CollisionRadius, Damage, ExplosionSoundSource).UpCast<Component>()));
                            }
                            else if (Type == "COMPONENT_ANIMATION")
                            {
                                xml_node<>* AnimationNode = ComponentNode->first_node("Animation");
                                std::vector<std::pair<int, std::vector<Ptr<Image>>>> Animations;
                                while (AnimationNode != nullptr)
                                {

                                    xml_node<>* AnimFPSNode = AnimationNode->first_node("AnimFPS");
                                    int AnimFPS = 1;
                                    if (AnimFPSNode != nullptr)
                                        AnimFPS = atoi(AnimFPSNode->first_attribute("value")->value());

                                    xml_node<>* FixImageRotationNode = AnimationNode->first_node("FixImageRotation");
                                    float FixAngle = 0.0f;
                                    if (FixImageRotationNode != nullptr)
                                        FixAngle = (float)(atof(FixImageRotationNode->first_attribute("value")->value()));

                                    xml_node<>* ImageNode = AnimationNode->first_node("Image");
                                    std::vector<Ptr<Image>> AnimImages;
                                    while (ImageNode != nullptr)
                                    {
                                        Ptr<Image> NewImage = ResourceManager::Instance()->LoadImage(ImageNode->first_attribute("path")->value(), FixAngle);
                                        AnimImages.push_back(NewImage);
                                        ImageNode = ImageNode->next_sibling("Image");
                                    }
                                    std::pair<int, std::vector<Ptr<Image>>> AnimationPair;
                                    AnimationPair.first = AnimFPS;
                                    AnimationPair.second = AnimImages;
                                    Animations.push_back(AnimationPair);
                                    AnimationNode = AnimationNode->next_sibling("Animation");
                                }

                                NewProjectile->AddComponent((CAnimation::Create(NewProjectile.UpCast<Entity>(), Animations).UpCast<Component>()));
                            }
                            ComponentNode = ComponentNode->next_sibling("Component");
                        }

                        EntityNode = EntityNode->next_sibling("Entity");
                    }
                }
            }
        }
    }
}

void Scene::ReadHUDXML(const String& Filename)
{
    // Read the XML file that contains the info of the HUD for the Player
    String Buffer = String::Read(Filename);
    if (Buffer != "")
    {
        // Read XML
        xml_document<> Doc;
        Doc.parse<0>((char*)Buffer.ToCString());
        xml_node<>* RootNode = Doc.first_node();

        if (RootNode != nullptr)
        {
            // Create an aux array to store the parent name of the widget
            std::vector<std::pair<const int, Ptr<Widget>>> AuxParentsVector;
            
            // Go through all HUD elements
            Ptr<Widget> NewWidget;
            xml_node<>* WidgetNode = RootNode->first_node("Widget");
            while (WidgetNode != nullptr)
            {
                int ID = atoi(WidgetNode->first_attribute("id")->value());
                String Type = "";
                if(WidgetNode->first_attribute("type"))
                    Type = WidgetNode->first_attribute("type")->value();
                int ParentID = -1;
                vec2 Position = vec2();
                float Rotation = 0.0f;
                vec2 Scale = vec2(1.0f, 1.0f);

                // Parent Widget ID
                xml_node<>* ParentIDNode = WidgetNode->first_node("ParentWidget");
                if (ParentIDNode != nullptr)
                {
                    ParentID = atoi(ParentIDNode->first_attribute("id")->value());
                }

                // Transform
                xml_node<>* TransformNode = WidgetNode->first_node("Transform");
                if (TransformNode != nullptr)
                {
                    xml_node<>* PositionNode = TransformNode->first_node("Position");
                    xml_node<>* RotationNode = TransformNode->first_node("Rotation");
                    xml_node<>* ScaleNode = TransformNode->first_node("Scale");
                    if (TransformNode != nullptr && PositionNode != nullptr && ScaleNode != nullptr)
                    {
                        Position = vec2((float)(atof(PositionNode->first_attribute("x")->value())), (float)(atof(PositionNode->first_attribute("y")->value())));
                        Rotation = (float)(atof(RotationNode->first_attribute("value")->value()));
                        Scale = vec2((float)(atof(ScaleNode->first_attribute("x")->value())), (float)(atof(ScaleNode->first_attribute("y")->value())));
                    }
                }

                if (Type == "PROGRESS_BAR")
                {
                    float Width = 0;
                    float Height = 0;
                    uint8 Red = 0;
                    uint8 Green = 0;
                    uint8 Blue = 0;
                    uint8 Alpha = 0;

                    // Size
                    xml_node<>* SizeNode = WidgetNode->first_node("Size");
                    if (SizeNode != nullptr)
                    {
                        Width = (float)(atof(SizeNode->first_attribute("width")->value()));
                        Height = (float)(atof(SizeNode->first_attribute("height")->value()));
                    }

                    // Color
                    xml_node<>* ColorNode = WidgetNode->first_node("Color");
                    if (ColorNode != nullptr)
                    {
                        Red = atoi(ColorNode->first_attribute("red")->value());
                        Green = atoi(ColorNode->first_attribute("green")->value());
                        Blue = atoi(ColorNode->first_attribute("blue")->value());
                        Alpha = atoi(ColorNode->first_attribute("alpha")->value());
                    }

                    // Create Widget Lifebar
                    Ptr<WLifebar> NewWLifeBar = WLifebar::Create(ID, Transform2D(Position, Rotation, Scale));
                    NewWLifeBar->SetWidth(Width);
                    NewWLifeBar->SetHeight(Height);
                    NewWLifeBar->SetColor(Red, Green, Blue, Alpha);

                    // Set Lifebar widget for each ship
                    size_t Size = mEntities.size();
                    for (uint32 i = 0; i < Size; ++i)
                    {
                        if (mEntities[i].DownCast<EShip>() != nullptr && mEntities[i].DownCast<EShip>()->GetLifeBarWidget() == nullptr)
                        {
                            mEntities[i].DownCast<EShip>()->SetLifeBarWidget(NewWLifeBar);
                            break;
                        }
                    }

                    NewWidget = NewWLifeBar.UpCast<Widget>();
                }

                else
                {
                    // Create Widget
                    NewWidget = Widget::Create(ID, Transform2D(Position, Rotation, Scale));
                }

                if (NewWidget != nullptr)
                {
                    // Components
                    xml_node<>* ComponentNode = WidgetNode->first_node("Component");
                    if (ComponentNode != nullptr)
                    {
                        String Type = ComponentNode->first_attribute("type")->value();

                        if (Type == "COMPONENT_ANIMATION")
                        {
                            xml_node<>* AnimationNode = ComponentNode->first_node("Animation");
                            std::vector<std::pair<int, std::vector<Ptr<Image>>>> Animations;
                            while (AnimationNode != nullptr)
                            {

                                xml_node<>* AnimFPSNode = AnimationNode->first_node("AnimFPS");
                                int AnimFPS = 1;
                                if (AnimFPSNode != nullptr)
                                    AnimFPS = atoi(AnimFPSNode->first_attribute("value")->value());

                                xml_node<>* FixImageRotationNode = AnimationNode->first_node("FixImageRotation");
                                float FixAngle = 0.0f;
                                if (FixImageRotationNode != nullptr)
                                    FixAngle = (float)(atof(FixImageRotationNode->first_attribute("value")->value()));

                                xml_node<>* ImageNode = AnimationNode->first_node("Image");
                                std::vector<Ptr<Image>> AnimImages;
                                while (ImageNode != nullptr)
                                {
                                    Ptr<Image> NewImage = ResourceManager::Instance()->LoadImage(ImageNode->first_attribute("path")->value(), FixAngle);
                                    AnimImages.push_back(NewImage);
                                    ImageNode = ImageNode->next_sibling("Image");
                                }
                                std::pair<int, std::vector<Ptr<Image>>> AnimationPair;
                                AnimationPair.first = AnimFPS;
                                AnimationPair.second = AnimImages;
                                Animations.push_back(AnimationPair);
                                AnimationNode = AnimationNode->next_sibling("Animation");
                            }
                            NewWidget->AddAnimationComponent((CAnimation::Create(nullptr, Animations)));
                        }
                    }
                    AuxParentsVector.push_back(std::pair<int, Ptr<Widget>>(ParentID, NewWidget));
                }
                WidgetNode = WidgetNode->next_sibling("Widget");
            }


            // After all elements are read, start defining the parent-child graph
            size_t Size = AuxParentsVector.size();
            for (uint32 i = 0; i < Size; ++i)
            {
                Ptr<Widget> ItWidget = AuxParentsVector[i].second;
                int ParentID = AuxParentsVector[i].first;
                if (ItWidget != nullptr)
                {
                    if (ParentID != -1 /* if has parent*/)
                    {
                        for (uint32 j = 0; j < Size; ++j)
                        {
                            Ptr<Widget> ItSecondWidget = AuxParentsVector[j].second;
                            if (ItSecondWidget != nullptr && ItSecondWidget->GetID() == ParentID)
                            {
                                ItSecondWidget->AddChildWidget(ItWidget);
                            }
                        }
                    }
                    else
                    {
                        mHUD = ItWidget;
                    }
                }
            }
            mHUD->Init();
        }
    }
}

void Scene::ReadBackgroundXML(const String & Filename)
{
    // Read the XML file that contains the info of the Entities that will be on the Background of the Scene (these entities do not interact with the Game Entities like ships or asteroids)
    String Buffer = String::Read(Filename);
    if (Buffer != "")
    {
        // Read XML
        xml_document<> Doc;
        Doc.parse<0>((char*)Buffer.ToCString());
        xml_node<>* RootNode = Doc.first_node();

        if (RootNode != nullptr)
        {
            // Use an aux vector to store all background entities, then take some of them set the background
            std::vector<Ptr<Entity>> AuxBackgroundEntities;

            // Go through all Background entities
            Ptr<Entity> NewEntity;
            xml_node<>* EntityNode = RootNode->first_node("Entity");
            while (EntityNode != nullptr)
            {
                String Type = EntityNode->first_attribute("type")->value();
                vec2 Position = vec2();
                float Rotation = 0.0f;
                vec2 Scale = vec2(1.0f, 1.0f);

                // Generate a random transform
                Position = vec2((float)(rand() % mSceneLimitMaxX + (mSceneLimitMinX + 20)), (float)(rand() % mSceneLimitMaxY + (mSceneLimitMinY + 20)));
                //float ScaleFact = ((float)(rand() % 10 + 1)) / 10.0f;
                //Scale = vec2(ScaleFact, ScaleFact);

                // Create the entity
                NewEntity = Entity::Create(Transform2D(Position, Rotation, Scale));

                if (World::IsEntityValid(NewEntity))
                {
                    AuxBackgroundEntities.push_back(NewEntity);

                    // Components
                    xml_node<>* ComponentNode = EntityNode->first_node("Component");
                    while (ComponentNode != nullptr)
                    {
                        String Type = ComponentNode->first_attribute("type")->value();
                        if (Type == "COMPONENT_ANIMATION")
                        {
                            xml_node<>* AnimationNode = ComponentNode->first_node("Animation");
                            std::vector<std::pair<int, std::vector<Ptr<Image>>>> Animations;
                            while (AnimationNode != nullptr)
                            {

                                xml_node<>* AnimFPSNode = AnimationNode->first_node("AnimFPS");
                                int AnimFPS = 1;
                                if (AnimFPSNode != nullptr)
                                    AnimFPS = atoi(AnimFPSNode->first_attribute("value")->value());

                                xml_node<>* FixImageRotationNode = AnimationNode->first_node("FixImageRotation");
                                float FixAngle = 0.0f;
                                if (FixImageRotationNode != nullptr)
                                    FixAngle = (float)(atof(FixImageRotationNode->first_attribute("value")->value()));

                                xml_node<>* ImageNode = AnimationNode->first_node("Image");
                                std::vector<Ptr<Image>> AnimImages;
                                while (ImageNode != nullptr)
                                {
                                    Ptr<Image> NewImage = ResourceManager::Instance()->LoadImage(ImageNode->first_attribute("path")->value(), FixAngle);
                                    AnimImages.push_back(NewImage);
                                    ImageNode = ImageNode->next_sibling("Image");
                                }
                                std::pair<int, std::vector<Ptr<Image>>> AnimationPair;
                                AnimationPair.first = AnimFPS;
                                AnimationPair.second = AnimImages;
                                Animations.push_back(AnimationPair);
                                AnimationNode = AnimationNode->next_sibling("Animation");
                            }

                            NewEntity->AddComponent((CAnimation::Create(NewEntity, Animations).UpCast<Component>()));
                        }
                        ComponentNode = ComponentNode->next_sibling("Component");
                    }

                    NewEntity->Init();
                }
                EntityNode = EntityNode->next_sibling("Entity");
            }

            // Select random entities for the background
            size_t Size = AuxBackgroundEntities.size();
            for (int i = 0; i < 7; ++i)
            {
                int RandomPick = rand() % Size;

                if (World::IsEntityValid(AuxBackgroundEntities[RandomPick]))
                    mEntities.push_back(AuxBackgroundEntities[RandomPick]);
            }
        }
    }
}

int Scene::GetProjectileCollectionIndex(EProjectile::ProjectileType Type) const
{
    // Get the index on the vector of the given Projectile Type
    size_t Size = mProjectileCollection.size();
    for (uint32 i = 0; i < Size; i++)
    {
        if (mProjectileCollection[i] != nullptr && mProjectileCollection[i]->GetProjectileType() == Type)
            return i;
    }
    return -1;
}

Ptr<EProjectile> Scene::GetProjectileInCollection(int Index)
{
    // Return a pointer to the Projectile, base on the index given
    if (Index > -1 && Index < ((int)mProjectileCollection.size()))
    {
        return mProjectileCollection[Index];
    }
    else
        return nullptr;
}

Ptr<EShip> Scene::GetOpponent(Ptr<EShip> Ship)
{
    // Used for AI: get a pointer to the opponent Ship
    size_t Size = mEntities.size();
    for(uint32 i = 0; i < Size; ++i)
    {
        if (World::IsEntityValid(mEntities[i]) && mEntities[i].DownCast<EShip>() != nullptr && mEntities[i].DownCast<EShip>() != Ship)
            return mEntities[i].DownCast<EShip>();
    }
    return nullptr;
}