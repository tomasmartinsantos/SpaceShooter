#ifndef IMOBSERVER_H
#define IMOBSERVER_H

#include "../Types.h"

class InputEvent;

class IIMObserver //InpuManager Observer interface
{
public:
    virtual void ManageEvent(const InputEvent& Event) = 0;

private:
    friend class Ptr<InputEvent>;
    friend class Ptr<const InputEvent>;
};

#endif