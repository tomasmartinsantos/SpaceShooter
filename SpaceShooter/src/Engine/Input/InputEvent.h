#ifndef INPUTEVENT_H
#define INPUTEVENT_H

class InputEvent
{
public:
    enum TEvent
    {
        EVENT_NONE,
        EVENT_MOUSE_RIGHT_CLICK,
        EVENT_MOUSE_LEFT_CLICK,
        EVENT_KEY_SPACE_PRESSED,
        EVENT_KEY_SPACE_RELEASED,
        EVENT_KEY_TAB_PRESSED,
        EVENT_KEY_TAB_RELEASED,
        EVENT_KEY_UP_PRESSED,
        EVENT_KEY_UP_RELEASED,
        EVENT_KEY_DOWN_PRESSED,
        EVENT_KEY_RIGHT_PRESSED,
        EVENT_KEY_RIGHT_RELEASED,
        EVENT_KEY_LEFT_PRESSED,
        EVENT_KEY_LEFT_RELEASED
    };
    InputEvent() { mTypeEvent = EVENT_NONE; }
    InputEvent(TEvent TypeEvent) { mTypeEvent = TypeEvent; }
    virtual TEvent GetTEvent() const { return mTypeEvent; }
private:
    TEvent mTypeEvent;
};

class EventMouseButtonClick : public InputEvent
{
    // Methods
public:
    EventMouseButtonClick() : InputEvent() { mX = 0; mY = 0; }
    EventMouseButtonClick(TEvent Event, int x, int y) : InputEvent(Event) { mX = x, mY = y; }
    int GetX() const { return mX; }
    int GetY() const { return mY; }

    // Variables
private:
    int mX, mY;
};

class EventKeyPressed : public InputEvent
{
public:
    EventKeyPressed() : InputEvent() {};
    EventKeyPressed(TEvent Event) : InputEvent(Event) {};
};
#endif