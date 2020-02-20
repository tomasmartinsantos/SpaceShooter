#include "../Glinclude.h"
#include "InputEvent.h"
#include "InputManager.h"
#include "IMObserver.h"
#include "Controller.h"


Ptr<Controller> Controller::Create()
{
    return new Controller();
}

/*************************** MOUSE CONTROLLER ***************************************/
Ptr<MouseController> MouseController::Create()
{
    return new MouseController();
}

MouseController::MouseController()
{
    mX = 0;
    mY = 0;
}

void MouseController::ProcessInput()
{
    glfwGetMousePos(&mX, &mY);
    EventMouseButtonClick NewInputEvent;

    if (MouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
    {
        NewInputEvent = EventMouseButtonClick(InputEvent::EVENT_MOUSE_LEFT_CLICK, mX, mY);
        IInputManagerController::Instance()->NotifyEvent(NewInputEvent);
    }
    if (MouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
    {
        NewInputEvent = EventMouseButtonClick(InputEvent::EVENT_MOUSE_RIGHT_CLICK, mX, mX);
        IInputManagerController::Instance()->NotifyEvent(NewInputEvent);
    }
}

bool MouseController::MouseButtonPressed(int button) const
{
    return glfwGetMouseButton(button) == GLFW_PRESS;
}

bool MouseController::MouseButtonReleased(int button) const
{
    return glfwGetMouseButton(button) == GLFW_RELEASE;
}


/**************************KEYBOARD CONTROLLER***************************************/
Ptr<KeyboardController> KeyboardController::Create()
{
    return new KeyboardController();
}

KeyboardController::KeyboardController()
{
    KeysPressed[GLFW_KEY_UP]       = false;
    KeysPressed[GLFW_KEY_RIGHT]    = false;
    KeysPressed[GLFW_KEY_LEFT]     = false;
    KeysPressed[GLFW_KEY_SPACE]    = false;
    KeysPressed[GLFW_KEY_TAB] = false;
}

void KeyboardController::ProcessInput()
{
    EventKeyPressed NewInputEvent;

    if (KeyPressed(GLFW_KEY_SPACE))
    {
        NewInputEvent = EventKeyPressed(InputEvent::EVENT_KEY_SPACE_PRESSED);
        IInputManagerController::Instance()->NotifyEvent(NewInputEvent);
    }
    if (KeyReleased(GLFW_KEY_SPACE))
    {
        NewInputEvent = EventKeyPressed(InputEvent::EVENT_KEY_SPACE_RELEASED);
        IInputManagerController::Instance()->NotifyEvent(NewInputEvent);
    }
    if (KeyPressed(GLFW_KEY_UP))
    {
        NewInputEvent = EventKeyPressed(InputEvent::EVENT_KEY_UP_PRESSED);
        IInputManagerController::Instance()->NotifyEvent(NewInputEvent);
    }
    if (KeyReleased(GLFW_KEY_UP))
    {
        NewInputEvent = EventKeyPressed(InputEvent::EVENT_KEY_UP_RELEASED);
        IInputManagerController::Instance()->NotifyEvent(NewInputEvent);
    }
    if (KeyPressed(GLFW_KEY_DOWN))
    {
        
        NewInputEvent = EventKeyPressed(InputEvent::EVENT_KEY_DOWN_PRESSED);
        IInputManagerController::Instance()->NotifyEvent(NewInputEvent);
    }
    if (KeyPressed(GLFW_KEY_RIGHT))
    {
        NewInputEvent = EventKeyPressed(InputEvent::EVENT_KEY_RIGHT_PRESSED);
        IInputManagerController::Instance()->NotifyEvent(NewInputEvent);
    }
    if (KeyReleased(GLFW_KEY_RIGHT))
    {
        NewInputEvent = EventKeyPressed(InputEvent::EVENT_KEY_RIGHT_RELEASED);
        IInputManagerController::Instance()->NotifyEvent(NewInputEvent);
    }
    if (KeyPressed(GLFW_KEY_LEFT))
    {
        NewInputEvent = EventKeyPressed(InputEvent::EVENT_KEY_LEFT_PRESSED);
        IInputManagerController::Instance()->NotifyEvent(NewInputEvent);
    }
    if (KeyReleased(GLFW_KEY_LEFT))
    {
        NewInputEvent = EventKeyPressed(InputEvent::EVENT_KEY_LEFT_RELEASED);
        IInputManagerController::Instance()->NotifyEvent(NewInputEvent);
    }
    if (KeyPressed(GLFW_KEY_TAB))
    {
        NewInputEvent = EventKeyPressed(InputEvent::EVENT_KEY_TAB_PRESSED);
        IInputManagerController::Instance()->NotifyEvent(NewInputEvent);
    }
    if (KeyReleased(GLFW_KEY_TAB))
    {
        NewInputEvent = EventKeyPressed(InputEvent::EVENT_KEY_TAB_RELEASED);
        IInputManagerController::Instance()->NotifyEvent(NewInputEvent);
    }
}

bool KeyboardController::KeyPressed(int key)
{
    if (glfwGetKey(key) == GLFW_PRESS)
    {
        KeysPressed[key] = true;
        return true;
    }
    else
        return false;
}

bool KeyboardController::KeyReleased(int key)
{
    if (KeysPressed[key] == true && glfwGetKey(key) == GLFW_RELEASE)
    {
        KeysPressed[key] = false;
        return true;
    }
    else
        return false;
}