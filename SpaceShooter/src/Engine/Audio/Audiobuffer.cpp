#include "Alinclude.h"
#include "../File.h"
#include "Audiobuffer.h"

Ptr<AudioBuffer> AudioBuffer::Create(const String& filename)
{
    return new AudioBuffer(filename);
}

AudioBuffer::AudioBuffer(const String& filename)
{
    mAlBuffer = 0;
    mFilename = filename;
    Ptr<File> f = File::Create(filename, FILE_READ);

    if (f != nullptr)
    {
        if (f->IsOpened())
        {
            //Header
            //ChunkID
            f->ReadInt8();
            f->ReadInt8();
            f->ReadInt8();
            f->ReadInt8();
            //RiffChunkSize
            f->ReadInt();
            //Format (“WAVE”)
            f->ReadInt8();
            f->ReadInt8();
            f->ReadInt8();
            f->ReadInt8();
            //SubChunkId (“fmt “)
            f->ReadInt8();
            f->ReadInt8();
            f->ReadInt8();
            f->ReadInt8();
            //FmtChunkSize
            int fmtChunkSize = f->ReadInt();
            //AudioFormat
            short audioFormat = f->ReadInt16();
            //Channels
            short channels = f->ReadInt16();
            //SampleRate
            int sampleRate = f->ReadInt();
            //ByteRate
            f->ReadInt();
            //BlockAlign
            f->ReadInt16();
            //BitsPerSample
            short bitsPerSample = f->ReadInt16();

            if (fmtChunkSize > 16)
            {
                //ExtraParamsSize
                short extraParamSize = f->ReadInt16();
                f->Seek(f->Pos() + extraParamSize);
            }


            char bloque[5];
            bloque[4] = 0;
            f->ReadBytes(bloque, 4);
            int size = 0;
            while (String(bloque) != "data")
            {
                size = f->ReadInt();
                f->Seek(f->Pos() + size);
                f->ReadBytes(bloque, 4);
            }

            //"data" found
            size = f->ReadInt();
            void* buffer = malloc(size);
            f->ReadBytes(buffer, size);

            alGenBuffers(1, &mAlBuffer);

            if (bitsPerSample == 8)
            {
                if (channels == 1)
                {
                    alBufferData(mAlBuffer, AL_FORMAT_MONO8, buffer, size, sampleRate);
                }
                else
                {
                    alBufferData(mAlBuffer, AL_FORMAT_STEREO8, buffer, size, sampleRate);
                }
            }
            else if (bitsPerSample == 16)
            {
                if (channels == 1)
                {
                    alBufferData(mAlBuffer, AL_FORMAT_MONO16, buffer, size, sampleRate);
                }
                else
                {
                    alBufferData(mAlBuffer, AL_FORMAT_STEREO16, buffer, size, sampleRate);
                }
            }

            delete buffer;
        }
        f->Close();
    }
}

AudioBuffer::~AudioBuffer()
{
    alDeleteBuffers(1, &mAlBuffer);
    mAlBuffer = 0;
}
