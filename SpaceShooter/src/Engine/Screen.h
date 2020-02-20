#ifndef UGINE_SCREEN_H
#define UGINE_SCREEN_H

#include "Types.h"
#include "String.h"


class Screen
{

    //Methods
public:
    static Ptr<Screen> Instance();
    
    // Pantalla
    virtual void Open(uint16 width, uint16 height, bool fullscreen);
    virtual void Close();
    virtual bool IsOpened() const { return mOpened; }
    virtual void SetTitle(const String& title);
    virtual void Refresh();
    virtual uint16 GetWidth() const { return mWidth; }
    virtual uint16 GetHeight() const { return mHeight; }
    virtual uint16 GetDesktopWidth() const;
    virtual uint16 GetDesktopHeight() const;

    // Temporizador
    float ElapsedTime() const { return mElapsed; }

    // Input
    virtual int32 GetMouseX() const { return mMousex; }
    virtual int32 GetMouseY() const { return mMousey; }
    virtual bool MouseButtonPressed(int button) const;
    virtual bool KeyPressed(int key) const;

private:
    static int CloseCallback();

protected:
    Screen();
    virtual ~Screen();

    // Variables
private:
    static Ptr<Screen> mInstance;

    bool    mOpened;
    uint16  mWidth, mHeight;
    int32   mMousex, mMousey;
    float   mLastTime;
    float   mElapsed;

    friend class Ptr<Screen>;
    friend class Ptr<const Screen>;
};

#endif
