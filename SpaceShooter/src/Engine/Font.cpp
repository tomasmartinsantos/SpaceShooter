#include "../Engine/Renderer.h"
#include "Font.h"


Ptr<Font> Font::Create(const String &filename)
{
    Ptr<Font> p = new Font(filename);
    p->mThis = p.UpCast<Image>();
    return p;
}

Font::Font(const String& filename) : Image(filename, 0, 16, 16)
{

}

uint16 Font::GetSize() const
{
    return GetHeight() * 2;
}

uint32 Font::GetTextWidth(const String& text) const
{
    uint32 textWidth = 0;
    textWidth = text.Length()*GetSize();
    return textWidth;
}

uint32 Font::GetTextHeight(const String& text) const
{
    uint32 textHeight = 0;
    textHeight = GetSize();
    return textHeight;
}

void Font::Render(const String& text, double x, double y) const
{
    for (int i = 0; i < text.Length(); i++)
    {
        Renderer::Instance()->DrawImage(mThis, x + i * GetSize(), y, text[i], GetSize(), GetSize(), 0);
    }
}