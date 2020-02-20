#include "Image.h"
#include "Font.h"
#include "Audio/Audiobuffer.h"
#include "Resourcemanager.h"


Ptr<ResourceManager> ResourceManager::mInstance = nullptr;

Ptr<ResourceManager> ResourceManager::Instance()
{
    if (mInstance == nullptr)
        mInstance = new ResourceManager();
    return mInstance;
}

ResourceManager::~ResourceManager()
{
    FreeResources();
}

Ptr<Font> ResourceManager::LoadFont(const String &filename)
{
    // Check if it is already loaded
    for ( uint32 i = 0; i < fonts.Size(); i++ )
        if ( fonts[i]->GetFilename() == filename )
            return fonts[i];

    // Load it
    Ptr<Font> font = Font::Create(filename);
    if (font !=nullptr && font->IsValid())
    {
        fonts.Add(font);
        return font;
    }
    else
        return nullptr;
}


Ptr<Image> ResourceManager::LoadImage(const String &filename, float FixImageRot, uint16 hframes, uint16 vframes)
{
    // Check if it is already loaded
    for (uint32 i = 0; i < images.Size(); i++)
    {
        if (images[i] != nullptr && images[i]->GetFilename() == filename)
            return images[i];
    }

    // Load it
    Ptr<Image> image = Image::Create(filename, FixImageRot, hframes, vframes);
    if (image != nullptr && image->IsValid())
    {
        images.Add(image);
        return image;
    } 
    else
        return nullptr;
}

Ptr<AudioBuffer> ResourceManager::LoadAudio(const String &filename)
{
    // Check if it is already loaded
    for (uint32 i = 0; i < audios.Size(); i++)
    {
        if (audios[i] != nullptr && audios[i]->GetFilename() == filename)
            return audios[i];
    }

    // Load it
    Ptr<AudioBuffer> audio = AudioBuffer::Create(filename);
    if (audio != nullptr && audio->IsValid())
    {
        audios.Add(audio);
        return audio;
    }
    else
        return nullptr;
}

void ResourceManager::FreeFonts()
{
    fonts.Clear();
}

void ResourceManager::FreeImages()
{
    images.Clear();
}

void ResourceManager::FreeAudios()
{
    audios.Clear();
}

void ResourceManager::FreeResources()
{
    FreeFonts();
    FreeImages();
    FreeAudios();
}
