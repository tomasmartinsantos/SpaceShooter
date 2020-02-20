#ifndef UGINE_MATH_H
#define UGINE_MATH_H

#include "Types.h"

template <typename T> T min(T a, T b) { return (a < b) ? a : b; }
template <typename T> T max(T a, T b) { return (a > b) ? a : b; }

float Log2(float x);
float DegSin(float degrees);
float DegCos(float degrees);
float DegTan(float degrees);
float DegASin(float sin);
float DegACos(float cos);
float DegATan(float tan);
float DegATan2(float y, float x);
float WrapValue(float val, float mod);
float Angle(float x1, float y1, float x2, float y2);
float Distance(float x1, float y1, float x2, float y2);
bool ValueInRange(float value, float min, float max);
bool PointInRect(float x, float y, float rectx, float recty, float width, float height);
void ClosestPointToRect(float x, float y, float rectx, float recty, float width, float height, float* outx, float* outy);
bool RectsOverlap(float x1, float y1, float width1, float height1, float x2, float y2, float width2, float height2);
void OverlappingRect(float x1, float y1, float width1, float height1, float x2, float y2, float width2, float height2, float* outx, float* outy, float* outwidth, float* outheight);
void TransformIsoCoords(float isoX, float isoY, float isoZ, float* screenX, float* screenY);

float SquareDistance(float x1, float y1, float x2, float y2);

float SqrDistance(const vec2& Pos1, const vec2& Pos2);
float AbsValue(float Value);
#endif
