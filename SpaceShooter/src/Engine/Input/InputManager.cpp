#include "InputEvent.h"
#include "IMObserver.h"
#include "Controller.h"
#include "InputManager.h"


Ptr<InputManager> InputManager::mInstance = nullptr;

Ptr<IInputManagerObserver> IInputManagerObserver::Instance()
{
    if (InputManager::mInstance == nullptr)
        InputManager::mInstance = new InputManager();
    return InputManager::mInstance.UpCast<IInputManagerObserver>();
}

Ptr<IInputManagerController> IInputManagerController::Instance()
{
    if (InputManager::mInstance == nullptr)
        InputManager::mInstance = new InputManager();
    return InputManager::mInstance.UpCast<IInputManagerController>();
}

InputManager::~InputManager()
{
    RemoveAllObservers();
    RemoveAllControllers();
}

void InputManager::AddObserver(Ptr<IIMObserver> Observer, const std::vector<InputEvent::TEvent>& Events)
{
    if(Observer != nullptr)
        mObservers.push_back(std::pair<Ptr<IIMObserver>, std::vector<InputEvent::TEvent>>(Observer, Events));
}

void InputManager::RemoveObserver(Ptr<IIMObserver> Observer)
{
    for (std::vector<std::pair<Ptr<IIMObserver>, std::vector<InputEvent::TEvent>>>::iterator it = mObservers.begin(); it != mObservers.end();)
    {
        if (it->first == Observer)
            it = mObservers.erase(it);
        else
            ++it;
    }
}

void InputManager::RemoveAllObservers()
{
    size_t Size = mObservers.size();
    for (uint32 i = 0; i < Size; ++i)
    {
        mObservers[i].first = nullptr;
    }
    mObservers.clear();
}

void InputManager::AddController(Ptr<Controller> Controller)
{
    if(Controller != nullptr)
        mControllers.push_back(Controller);
}

void InputManager::RemoveController(Ptr<Controller> _Controller)
{
    for (std::vector<Ptr<Controller>>::iterator it = mControllers.begin(); it != mControllers.end();)
    {
        if ((*it) == _Controller)
            it = mControllers.erase(it);
        else
            ++it;
    }
}

void InputManager::RemoveAllControllers()
{
    size_t Size = mControllers.size();
    for(uint32 i = 0; i < Size; ++i)
    {
        mControllers[i] = nullptr;
    }
    mControllers.clear();
}

void InputManager::ProcessInput()
{
    size_t Size = mControllers.size();
    for (uint32 i = 0; i < Size; ++i)
    {
        if(mControllers[i] != nullptr)
            mControllers[i]->ProcessInput();
    }
}

void InputManager::NotifyEvent(const InputEvent& Event)
{
    size_t Size = mObservers.size();
    for (uint32 i = 0; i < Size; ++i)
    {
        if (mObservers[i].first != nullptr)
        {
            size_t Size2 = mObservers[i].second.size();
            for (uint32 j = 0; j < Size2; ++j)
            {
                if (mObservers[i].first != nullptr  && mObservers[i].second[j] == Event.GetTEvent())
                    mObservers[i].first->ManageEvent(Event);
            }
        }
    }
}