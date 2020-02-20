#ifndef LISTENER_H
#define LISTENER_H

#include "../Types.h"
#include <stdlib.h>

class Listener
{
    // Methods
public:
    static Ptr<Listener> Instance();

    void SetPosition(float x, float y, float z);
    void SetVelocity(float x, float y, float z);
    void SetOrientation(float x, float y, float z);
protected:
    Listener();
    ~Listener() {}

    // Variables
private:
    static Ptr<Listener> mInstance;

    friend class Ptr<Listener>;
    friend class Ptr<const Listener>;
};

#endif