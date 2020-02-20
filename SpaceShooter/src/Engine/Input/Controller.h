#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../Types.h"


class Controller
{
    // Methods
public:
    static Ptr<Controller> Create();

    virtual void ProcessInput() {};

protected:
    Controller() {}
    virtual ~Controller() {}

private:
    friend class Ptr<Controller>;
    friend class Ptr<const Controller>;
};

class MouseController : public Controller
{
    // Methods
public:
    static Ptr<MouseController> Create();

    virtual void ProcessInput();
    int GetMouseX() const { return mX; }
    int GetMouseY() const { return mY; }
    bool MouseButtonPressed(int button) const;
    bool MouseButtonReleased(int button) const;

protected:
    MouseController();
    virtual ~MouseController() {}

    // Variables
private:
    int mX, mY;

    friend class Ptr<MouseController>;
    friend class Ptr<const MouseController>;
};

class KeyboardController : public Controller
{
    // Methods
public:
    static Ptr<KeyboardController> Create();

    virtual void ProcessInput();
    bool KeyPressed(int button);
    bool KeyReleased(int button);

protected:
    KeyboardController();
    virtual ~KeyboardController() {}

private:
    std::map<int, bool> KeysPressed;

    friend class Ptr<KeyboardController>;
    friend class Ptr<const KeyboardController>;
};

#endif