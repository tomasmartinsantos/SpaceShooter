#include "Glinclude.h"
#include "Image.h"
#include "Font.h"
#include "Math.h"
#include <math.h>
#include "Renderer.h"

#define ELLIPSEPOINTS 32

Ptr<Renderer> Renderer::mInstance = nullptr;

Ptr<Renderer> Renderer::Instance()
{
    if (mInstance == nullptr)
        mInstance = new Renderer();
    return mInstance;
}

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

void Renderer::SetBlendMode(BlendMode blendMode) const
{
    switch ( blendMode )
    {
    case SOLID:
        glBlendFunc(GL_ONE, GL_ZERO);
        break;
    case ALPHA:
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        break;
    case MULTIPLICATIVE:
        glBlendFunc(GL_DST_COLOR, GL_ZERO);
        break;
    case ADDITIVE:
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        break;
    }
}

void Renderer::SetColor(uint8 r, uint8 g, uint8 b, uint8 a) const
{
    glColor4ub(r, g, b, a);
}

void Renderer::SetOrigin(double x, double y) const
{
    glLoadIdentity();
    glTranslated((GLdouble)-x, (GLdouble)-y, 0);
}

void Renderer::Clear(uint8 r, uint8 g, uint8 b) const
{
    glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, 0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::DrawPoint(double x, double y) const
{
    GLdouble vertices[] = {x, y};
    GLdouble texCoords[] = {0, 0};
    glBindTexture(GL_TEXTURE_2D, 0);
    glVertexPointer(2, GL_DOUBLE, 0, vertices);
    glTexCoordPointer(2, GL_DOUBLE, 0, texCoords);
    glDrawArrays(GL_POINTS, 0, 1);
}

void Renderer::DrawLine(double x1, double y1, double x2, double y2) const
{
    GLdouble vertices[] = {x1, y1, x2, y2};
    GLdouble texCoords[] = {0, 0, 0, 0};
    glBindTexture(GL_TEXTURE_2D, 0);
    glVertexPointer(2, GL_DOUBLE, 0, vertices);
    glTexCoordPointer(2, GL_DOUBLE, 0, texCoords);
    glDrawArrays(GL_LINES, 0, 2);
}

void Renderer::DrawRect(double x, double y, double width, double height) const
{
    GLdouble vertices[] = {x, y, x+width, y, x, y+height, x+width, y+height};
    GLdouble texCoords[] = {0, 0, 0, 0, 0, 0, 0, 0};
    glBindTexture(GL_TEXTURE_2D, 0);
    glVertexPointer(2, GL_DOUBLE, 0, vertices);
    glTexCoordPointer(2, GL_DOUBLE, 0, texCoords);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Renderer::DrawEllipse(double x, double y, double xradius, double yradius) const
{
    GLdouble vertices[ELLIPSEPOINTS*2];
    for ( uint32 i = 0; i < ELLIPSEPOINTS; i++ )
    {
        vertices[i*2] = x + (((double)DegCos(i * 360.0f / ELLIPSEPOINTS)) * xradius);
        vertices[i*2+1] = y + (((double)DegSin(i * 360.0f / ELLIPSEPOINTS)) * yradius);
    }

    GLdouble texCoords[ELLIPSEPOINTS*2];
    memset(texCoords, 0, ELLIPSEPOINTS*2*sizeof(GLdouble));

    glBindTexture(GL_TEXTURE_2D, 0);
    glVertexPointer(2, GL_DOUBLE, 0, vertices);
    glTexCoordPointer(2, GL_DOUBLE, 0, texCoords);
    glDrawArrays(GL_TRIANGLE_FAN, 0, ELLIPSEPOINTS);
}


void Renderer::DrawImage(const Ptr<Image> image, double x, double y, uint32 frame, double width, double height, double ang) const
{
    if (image != nullptr)
    {
        if (width == 0 || height == 0)
        {
            width = image->GetWidth();
            height = image->GetHeight();
        }

        uint32 row = (uint32)floor(frame * 1.0 / uint32(image->GetHFrames()));
        uint32 column = frame % image->GetHFrames();
        GLdouble handlexscale = width / image->GetWidth();
        GLdouble handleyscale = height / image->GetHeight();
        GLdouble ustep = image->GetLastU() / image->GetHFrames();
        GLdouble vstep = image->GetLastV() / image->GetVFrames();
        GLdouble ubegin = ustep * column;
        GLdouble vbegin = vstep * row;
        GLdouble uend = ubegin + ustep;
        GLdouble vend = vbegin + vstep;
        GLdouble x0 = -(image->GetHandleX()*handlexscale);
        GLdouble y0 = -(image->GetHandleY()*handleyscale);
        GLdouble x1 = -(image->GetHandleX()*handlexscale) + width;
        GLdouble y1 = -(image->GetHandleY()*handleyscale) + height;
        GLdouble vertices[] = { x0, y0, x1, y0, x0, y1, x1, y1 };
        GLdouble texCoords[] = { ubegin, vbegin, uend, vbegin, ubegin, vend, uend, vend };

        glPushMatrix();
        glTranslated(x, y, 0);
        glRotated(ang, 0, 0, -1);
        image->Bind();
        glVertexPointer(2, GL_DOUBLE, 0, vertices);
        glTexCoordPointer(2, GL_DOUBLE, 0, texCoords);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glPopMatrix();
    }
}

void Renderer::DrawText(const Ptr<Font> font, const String &text, double x, double y) const
{
    if(font != nullptr)
        font->Render(text, x, y);
}


uint32 Renderer::GenImage(uint8* buffer, uint16 width, uint16 height) const
{
    uint32 texId;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    return texId;
}

void Renderer::BindImage(uint32 glhandle) const
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, glhandle);
}

void Renderer::DeleteImage(uint32 glhandle) const
{
    glDeleteTextures(1, &glhandle);
}

void Renderer::PushMatrix() const
{
    glPushMatrix();
}
void Renderer::PopMatrix() const
{
    glPopMatrix();
}

void Renderer::TranslateMatrix(double x, double y, double z) const
{
    glTranslated((GLdouble)x, (GLdouble)y, (GLdouble)z);
}

void Renderer::RotateMatrix(double angle, double x, double y, double z) const
{
    glRotated((GLdouble)angle, (GLdouble)x, (GLdouble)y, (GLdouble)z);
}

void Renderer::TexImage2D(uint16 width, uint16 height, uint8 *buffer) const
{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
}