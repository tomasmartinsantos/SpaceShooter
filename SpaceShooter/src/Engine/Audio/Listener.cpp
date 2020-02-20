#include "Alinclude.h"
#include "Listener.h"

Ptr<Listener> Listener::mInstance = nullptr;

Ptr<Listener> Listener::Instance()
{
    if (mInstance == nullptr)
        mInstance = new Listener();
    return mInstance;
}

Listener::Listener()
{
    SetPosition(0, 0, 0);
    SetVelocity(0, 0, 0);
    SetOrientation(0, 0, 0);
}

void Listener::SetPosition(float x, float y, float z)
{
    alListener3f(AL_POSITION, x, y, z);
}

void Listener::SetVelocity(float x, float y, float z)
{
    alListener3f(AL_VELOCITY, x, y, z);
}

void Listener::SetOrientation(float x, float y, float z)
{
    alListener3f(AL_ORIENTATION, x, y, z);
}