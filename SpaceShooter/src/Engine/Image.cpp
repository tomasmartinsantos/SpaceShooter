#include "Glinclude.h"
#include "Math.h"
#include <math.h>
#include <stdlib.h>
#include "Renderer.h"
#include "Image.h"


extern "C"
{
    unsigned char *stbi_load(const char *filename, int *x, int *y, int *comp, int req_comp);
    void stbi_image_free(void *buffer);
}

Ptr<Image> Image::Create(const String &filename, float FixImageRot, uint16 hframes, uint16 vframes)
{
    Ptr<Image> p = new Image(filename, FixImageRot, hframes, vframes);
    p->mThis = p;
    return p;
}

Image::Image(const String &filename, float FixImageRot, uint16 hframes, uint16 vframes)
{
    mFilename = filename;
    mHframes = hframes;
    mVframes = vframes;
    mFixAngleRotation = FixImageRot;
    mWidth = 0;
    mHeight = 0;
    mHandlex = 0;
    mHandley = 0;
    mGltex = 0;
    mLastU = 1.0;
    mLastV = 1.0;

    int width_2 = 0;
    int height_2 = 0;

    uint8 *buffer = stbi_load(filename.ToCString(), &width_2, &height_2, nullptr, 4);
    mWidth = static_cast <uint16> (width_2);
    mHeight = static_cast <uint16> (height_2);

    uint16 newWidth = static_cast <uint16> (pow(2, ceil(Log2(mWidth))));
    uint16 newHeight = static_cast <uint16> (pow(2, ceil(Log2(mHeight))));

    int frameWidth = GetWidth();
    int frameHeight = GetHeight();
    uint8 *iteratorFrame = buffer;
    uint8 *p_frameX = buffer;
    uint8 *p_frameY = buffer;

    // Look for the RGB(255, 0, 255) (background "pink") and set the A channel to 0.
    if (buffer)
    {
        for (uint32 i = 0; i < GetNumFrames(); i++)
        {
            for (int y = 0; y < frameHeight; y++)
            {
                for (int x = 0; x < frameWidth; x++)
                {
                    //Pixel pink
                    if (iteratorFrame[0] == 255 && iteratorFrame[1] == 0 && iteratorFrame[2] == 255)
                        iteratorFrame[3] = 0;
                    iteratorFrame = iteratorFrame + 4;
                }
                p_frameY = p_frameY + width_2 * 4;
                iteratorFrame = p_frameY;
            }
            if (WrapValue((float)i, (float)GetHFrames()) == GetHFrames() - 1)
            {
                p_frameX = p_frameX + (frameWidth * 4) + (frameHeight - 1) * width_2 * 4;
                p_frameY = p_frameX;
                iteratorFrame = p_frameX;
            }
            else
            {
                p_frameX = p_frameX + frameWidth * 4;
                p_frameY = p_frameX;
                iteratorFrame = p_frameX;
            }
        }
    }

    if (mWidth != newWidth || mHeight != newHeight)
    {
        mLastU = (double)mWidth / newWidth;
        mLastV = (double)mHeight / newHeight;


        uint8 *newBuffer = (uint8 *)calloc(newWidth*newHeight * 4, sizeof(uint8));
        if (buffer && newBuffer)
        {
            for (int i = 0; i < mHeight; i++)
            {
                memcpy(newBuffer + (i*newWidth - 1) * 4, buffer + (i*mWidth - 1) * 4, sizeof(uint8)*mWidth * 4);
            }

            mGltex = Renderer::Instance()->GenImage(newBuffer, newWidth, newHeight);
            stbi_image_free(newBuffer);
            stbi_image_free(buffer);
        }
    }
    else
    {
        // Generate texture
        if (buffer)
        {
            mGltex = Renderer::Instance()->GenImage(buffer, mWidth, mHeight);
            stbi_image_free(buffer);
        }
    }
}

Image::~Image()
{
    Renderer::Instance()->DeleteImage(mGltex);
}

void Image::Bind() const
{
    Renderer::Instance()->BindImage(mGltex);
}
