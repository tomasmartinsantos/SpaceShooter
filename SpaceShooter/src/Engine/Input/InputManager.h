#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "../Types.h"
#include "InputEvent.h"


class IIMObserver;
class Controller;

class IInputManagerObserver
{
public:
    static Ptr<IInputManagerObserver> Instance();

    virtual void AddObserver(Ptr<IIMObserver> Observer, const std::vector<InputEvent::TEvent>& Events) = 0;
    virtual void RemoveObserver(Ptr<IIMObserver> Observer) = 0;
    virtual void RemoveAllObservers() = 0;

private:
    friend class Ptr<IInputManagerObserver>;
    friend class Ptr<const IInputManagerObserver>;
};

class IInputManagerController
{
public:
    static Ptr<IInputManagerController> Instance();

    virtual void AddController(Ptr<Controller> Controller) = 0;
    virtual void RemoveController(Ptr<Controller> Controller) = 0;
    virtual void RemoveAllControllers() = 0;
    virtual void ProcessInput() = 0;
    virtual void NotifyEvent(const InputEvent& Event) = 0;

private:
    friend class Ptr<IInputManagerController>;
    friend class Ptr<const IInputManagerController>;
};

class InputManager : public IInputManagerObserver, IInputManagerController
{
    // Methods
public:
    virtual void AddObserver(Ptr<IIMObserver> Observer, const std::vector<InputEvent::TEvent>& Events);
    virtual void RemoveObserver(Ptr<IIMObserver> Observer);
    virtual void RemoveAllObservers();
    virtual void AddController(Ptr<Controller> Controller);
    virtual void RemoveController(Ptr<Controller> Controller);
    virtual void RemoveAllControllers();
    virtual void ProcessInput();
    virtual void NotifyEvent(const InputEvent& Event);

protected:
    InputManager() {}
    ~InputManager();

    // Variables
private:
    static Ptr<InputManager> mInstance;

    std::vector<Ptr<Controller>> mControllers;
    std::vector<std::pair<Ptr<IIMObserver>, std::vector<InputEvent::TEvent>>> mObservers;

    friend class IInputManagerController;
    friend class IInputManagerObserver;
    friend class Ptr<InputManager>;
    friend class Ptr<const InputManager>;
};

#endif