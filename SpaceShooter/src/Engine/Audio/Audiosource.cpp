#include "Alinclude.h"
#include "Audiobuffer.h"
#include "AudioSource.h"

Ptr<AudioSource> AudioSource::Create(Ptr<AudioBuffer> Buffer)
{
    return new AudioSource(Buffer);
}

AudioSource::AudioSource(Ptr<AudioBuffer> Buffer)
{
    alGenSources(1, &mSource);
    SetLooping(false);
    SetPitch(1.0f);
    SetGain(1.0f);
    SetPosition(0, 0, 0);
    SetVelocity(0, 0, 0);

    mBuffer = Buffer;
    alSourcei(mSource, AL_BUFFER, mBuffer->GetBuffer());
}

AudioSource::~AudioSource()
{
    alDeleteSources(1, &mSource);
}

void AudioSource::SetPitch(float pitch)
{
    alSourcef(mSource, AL_PITCH, pitch);
}

void AudioSource::SetGain(float gain)
{
    alSourcef(mSource, AL_GAIN, gain);
}

void AudioSource::SetLooping(bool loop)
{
    alSourcei(mSource, AL_LOOPING, loop);
}

void AudioSource::SetPosition(float x, float y, float z)
{
    alSource3f(mSource, AL_POSITION, x, y, z);
}

void AudioSource::SetVelocity(float x, float y, float z)
{
    alSource3f(mSource, AL_VELOCITY, x, y, z);
}

void AudioSource::Play()
{
    alSourcePlay(mSource);
}

void AudioSource::Stop()
{
    alSourceStop(mSource);
}

void AudioSource::Pause()
{
    alSourcePause(mSource);
}

bool AudioSource::IsPlaying() const
{
    ALint ret = 0;
    alGetSourcei(mSource, AL_SOURCE_STATE, &ret);
    return(ret == AL_PLAYING);
}