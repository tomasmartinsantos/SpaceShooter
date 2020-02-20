#include "../Engine/Screen.h"
#include "../Engine/Resourcemanager.h"
#include "Scene.h"
#include "../Engine/Input/Controller.h"
#include "../Engine/Input/InputManager.h"
#include "../Engine/Renderer.h"
#include "../Engine/Glinclude.h"
#include "Entities/Entity.h"
#include "../Engine/Audio/Audiomanager.h"
#include "Components\HeadersComponents.h"
#include "../Engine/Audio/Audiobuffer.h"
#include "../Engine/Audio/Audiosource.h"
#include "../Engine/Image.h"
#include "../Engine/Font.h"
#include "../Engine/Input/IMObserver.h"
#include "World.h"


Ptr<World> World::mInstance = nullptr;

Ptr<World> World::Instance()
{
    if (mInstance == nullptr)
        mInstance = new World();
    return mInstance;
}

World::World()
{

}

World::~World()
{
    IInputManagerController::Instance()->RemoveAllControllers();
}

void World::Main()
{
    /* initialize random seed: */
    srand((unsigned int)time(NULL));
    
    Screen::Instance()->Open(900, 600, false);

    // Initialize audio engine
    AudioEngine::Instance()->Init();

    // Controllers
    Ptr<MouseController> Mouse = MouseController::Create();
    IInputManagerController::Instance()->AddController(Mouse.UpCast<Controller>());
    Ptr<KeyboardController> Keyboard = KeyboardController::Create();
    IInputManagerController::Instance()->AddController(Keyboard.UpCast<Controller>());

    // Initiate Scene
    mCurrentScene = Scene::Create();
    mCurrentScene->Init("data/Scenes/scene_01.xml");

    while (Screen::Instance()->IsOpened() && !(Screen::Instance()->KeyPressed(GLFW_KEY_ESC)))
    {
        // MAIN GAME LOOP

        Renderer::Instance()->Clear(0, 0, 0);

        ProcessInput();
        Update();
        Render();
        CheckScene();

        Screen::Instance()->Refresh();
    }

    EndWorld();
    ResourceManager::Instance()->FreeResources();
    AudioEngine::Instance()->Finish();
}

void World::CheckScene()
{
    // Check if Scene is in "CHANGING" state
    if (mCurrentScene != nullptr && mCurrentScene->GetSceneState() == Scene::CHANGING_SCENE)
        ChangeScene();
}

void World::ChangeScene()
{
    // End current state and initiate a new one
    if(mCurrentScene != nullptr)
        mCurrentScene->EndScene();

    mCurrentScene = Scene::Create();
    mCurrentScene->Init("data/Scenes/scene_01.xml");
}

void World::ProcessInput()
{
    if (mCurrentScene != nullptr)
        mCurrentScene->ProcessInput();
}

void World::Update()
{
    if (mCurrentScene != nullptr)
        mCurrentScene->Update(Screen::Instance()->ElapsedTime());
}

void World::Render()
{
    if (mCurrentScene != nullptr)
        mCurrentScene->Render();

}

void World::EndWorld()
{
    if (mCurrentScene != nullptr)
        mCurrentScene->EndScene();
    IInputManagerController::Instance()->RemoveAllControllers();
}

bool World::IsEntityValid(Ptr<Entity> _Entity)
{
    return _Entity != nullptr && !_Entity->IsPendingKill();
}