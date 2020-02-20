#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include "../Types.h"
#include <stdlib.h>

class AudioEngine
{
    // Methods
public:
    static Ptr<AudioEngine> Instance();
    void Init();
    void Finish();

protected:
    AudioEngine();
    ~AudioEngine();

    // Variables
private:
    static Ptr<AudioEngine> mInstance;
    void* mDevice;
    void* mContext;

    friend class Ptr<AudioEngine>;
    friend class Ptr<const AudioEngine>;
};

#endif
