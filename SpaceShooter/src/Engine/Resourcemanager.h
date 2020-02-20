#ifndef UGINE_RESOURCEMANAGER_H
#define UGINE_RESOURCEMANAGER_H

#include "Types.h"
#include "Array.h"
#include "String.h"


class Font;
class Image;
class AudioBuffer;

class ResourceManager
{
    // Methods
public:
    static Ptr<ResourceManager> Instance();

    virtual Ptr<Font> LoadFont(const String& filename);
    virtual Ptr<Image> LoadImage(const String& filename, float FixImageRot = 0.0f, uint16 hframes = 1, uint16 vframes = 1);
    virtual Ptr<AudioBuffer> LoadAudio(const String& filename);
    virtual void FreeFonts();
    virtual void FreeImages();
    virtual void FreeAudios();
    virtual void FreeResources();

protected:
    ResourceManager() {}
    virtual ~ResourceManager();

    // Variables
private:
    static Ptr<ResourceManager> mInstance;

    Array<Ptr<Font>> fonts;
    Array<Ptr<Image>> images;
    Array<Ptr<AudioBuffer>> audios;

    friend class Ptr<ResourceManager>;
    friend class Ptr<const ResourceManager>;
};

#endif
