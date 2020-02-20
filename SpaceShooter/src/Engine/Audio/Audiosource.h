#ifndef AUDIOSOURCE_H
#define AUDIOSOURCE_H

#include "../Types.h"


class AudioBuffer;

class AudioSource
{
    // Methods
public:
    static Ptr<AudioSource> Create(Ptr<AudioBuffer> Buffer);

    void SetPitch(float pitch);
    void SetGain(float gain);
    void SetLooping(bool loop);
    void SetPosition(float x, float y, float z);
    void SetVelocity(float x, float y, float z);
    void Play();
    void Stop();
    void Pause();
    bool IsPlaying() const;

protected:
    AudioSource(Ptr<AudioBuffer> buffer);
    ~AudioSource();

    // Variables
private:
    unsigned int mSource;
    Ptr<AudioBuffer> mBuffer;

    friend class Ptr<AudioSource>;
    friend class Ptr<const AudioSource>;
};
#endif
