#ifndef AUDIOBUFFER_H
#define AUDIOBUFFER_H

#include "../Types.h"
#include "../String.h"

class AudioBuffer
{
    // Methods
public:
    static Ptr<AudioBuffer> Create(const String& filename);

    bool IsValid()              const { return mAlBuffer != 0; }
    unsigned int GetBuffer()    const { return mAlBuffer; }
    const String& GetFilename() const { return mFilename; }

protected:
    AudioBuffer(const String& filename);
    ~AudioBuffer();

    // Variables
private:
    unsigned int mAlBuffer;
    String       mFilename;

    friend class Ptr<AudioBuffer>;
    friend class Ptr<const AudioBuffer>;
};

#endif

