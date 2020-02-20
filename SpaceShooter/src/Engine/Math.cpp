#include <math.h>
#include "Types.h"
#include "Math.h"


#define DEG2RAD 0.0174532925
#define RAD2DEG 57.2957795

float Log2(float x) {
    return log(x) / log(2.0f);
}

float DegSin(float degrees) {
    return (float)sin(DEG2RAD * degrees);
}

float DegCos(float degrees) {
    return (float)cos(DEG2RAD * degrees);
}

float DegTan(float degrees) {
    return (float)tan(DEG2RAD * degrees);
}

float DegASin(float sin) {
    return (float)(asin(sin) * RAD2DEG);
}

float DegACos(float cos) {
    return (float)(acos(cos) * RAD2DEG);
}

float DegATan(float tan) {
    return (float)(atan(tan) * RAD2DEG);
}

float DegATan2(float y, float x) {
    return (float)(atan2(y, x) * RAD2DEG);
}

float WrapValue(float val, float mod) {
    if (mod == 0) return val;
    return val - mod*floor(val/mod);
}


float Angle(float x1, float y1, float x2, float y2) {
    return WrapValue(DegATan2(y1-y2,x2-x1),360);
}

float Distance(float x1, float y1, float x2, float y2) {
    return sqrt(pow((x2-x1),2)+pow((y2-y1),2));
}

bool ValueInRange(float value, float min, float max) {
    return (value >= min) && (value <= max);
}

bool PointInRect(float x, float y, float rectx, float recty, float width, float height) {
    return (ValueInRange(x, rectx, rectx + width) && ValueInRange(y, recty, recty + height));
}

void ClosestPointToRect(float x, float y, float rectx, float recty, float width, float height, float* outx, float* outy) {
    *outx = (x < rectx) ? rectx : (x > rectx+width) ? rectx+width : x;
    *outy = (y < recty) ? recty : (y > recty+height) ? recty+height : y;
}

bool RectsOverlap(float x1, float y1, float width1, float height1, float x2, float y2, float width2, float height2) {
    if (PointInRect(x1, y1, x2, y2, width2, height2))
        return true;
    else if (PointInRect(x1 + width1, y1, x2, y2, width2, height2))
        return true;
    else if (PointInRect(x1, y1 + height1, x2, y2, width2, height2))
        return true;
    else if (PointInRect(x1 + width1, y1 + height1, x2, y2, width2, height2))
        return true;
    else if (PointInRect(x2, y2, x1, y1, width1, height1))
        return true;
    else if (PointInRect(x2 + width2, y2, x1, y1, width1, height1))
        return true;
    else if (PointInRect(x2, y2 + height2, x1, y1, width1, height1))
        return true;
    else if (PointInRect(x2 + width2, y2 + height2, x1, y1, width1, height1))
        return true;
    else
        return false;
}

void OverlappingRect(float x1, float y1, float width1, float height1, float x2, float y2, float width2, float height2, float* outx, float* outy, float* outwidth, float* outheight) {
    if (ValueInRange(x1, x2, x2 + width2))
        *outx = x1;
    else
        *outx = x2;
    if (ValueInRange(y1, y2, y2 + height2))
        *outy = y1;
    else
        *outy = y2;
    if (ValueInRange(x1 + width1, x2, x2 + width2))
        *outwidth = x1 + width1 - *outx;
    else
        *outwidth = x2 + width2 - *outx;
    if (ValueInRange(y1 + height1, y2, y2 + height2))
        *outheight = y1 + height1 - *outy;
    else
        *outheight = y2 + height2 - *outy;
}

void TransformIsoCoords(float isoX, float isoY, float isoZ, float* screenX, float* screenY) {
}
float SquareDistance(float x1, float y1, float x2, float y2)
{
    return pow((x2 - x1), 2) + pow((y2 - y1), 2);
}

float SqrDistance(const vec2& Pos1, const vec2& Pos2)
{
    return powf((Pos1.x - Pos2.x), 2) + powf((Pos1.y - Pos2.y), 2);
}

float AbsValue(float Value)
{
    float ret = Value;
    if (Value < 0)
        ret *= -1;
    return ret;
}