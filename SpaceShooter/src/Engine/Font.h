#ifndef UGINE_FONT_H
#define UGINE_FONT_H

#include "Types.h"
#include "Array.h"
#include "String.h"
#include "Image.h"

class Font : public Image
{
    // Methods
public:
    static Ptr<Font> Create(const String& filename);

    virtual uint16 GetSize() const;
    virtual uint32 GetTextWidth(const String& text) const;
    virtual uint32 GetTextHeight(const String& text) const;
    virtual void Render(const String& text, double x, double y) const;

protected:
    Font(const String& filename);

    // Variables
private:

    friend class Ptr<Font>;
    friend class Ptr<const Font>;
};

#endif

