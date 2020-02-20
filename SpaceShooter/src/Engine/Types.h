#ifndef UGINE_TYPES_H
#define UGINE_TYPES_H

typedef signed char int8;
typedef signed short int16;
typedef signed int int32;
typedef signed long int64;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long uint64;

#define HUD_SIZE 200

#include "Smartptr.h"
#include <time.h>
#include <vector>
#include <map>


struct vec2
{
    float x, y;
    vec2()
    {
        x = 0.0f;
        y = 0.0f;
    }
    vec2(float _x, float _y)
    {
        x = _x;
        y = _y;
    }
};

struct vec3
{
    float x, y, z;
    vec3()
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }
    vec3(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }
};

struct Transform2D
{
    vec2    Position;
    float   Rotation;
    vec2    Scale;
    Transform2D()
    {
        Position    = vec2();
        Rotation    = 0.0f;
        Scale       = vec2(1.0f, 1.0f);
    }
    Transform2D(const vec2& _Pos, float _Rot = 0.0f, const vec2& _Sca = vec2(1.0f, 1.0f))
    {
        Position = _Pos;
        Rotation = _Rot;
        Scale = _Sca;
    }
};

#endif