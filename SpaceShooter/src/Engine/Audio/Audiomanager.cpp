#include "Alinclude.h"
#include "Audiomanager.h"


Ptr<AudioEngine> AudioEngine::mInstance = nullptr;

Ptr<AudioEngine> AudioEngine::Instance()
{
    if (mInstance == nullptr)
        mInstance = new AudioEngine();
    return mInstance;
}

AudioEngine::AudioEngine()
{
    mDevice     = nullptr;
    mContext    = nullptr;
}

AudioEngine::~AudioEngine()
{
    mDevice     = nullptr;
    mContext    = nullptr;
}


void AudioEngine::Init()
{
    mDevice = alcOpenDevice(nullptr);
    mContext = alcCreateContext((ALCdevice*)(mDevice), nullptr);
    alcMakeContextCurrent((ALCcontext*)(mContext));
}

void AudioEngine::Finish()
{
    alcDestroyContext((ALCcontext*)(mContext));
    alcCloseDevice((ALCdevice*)(mDevice));
}