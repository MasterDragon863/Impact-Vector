#pragma once
#include "raylib.h"

struct Collider
{
    Vector2 origin;
    float _minX;
    float _minY;
    float _maxX;
    float _maxY;
    float halfWidth;
    float halfHeight;

    inline Vector2 getMin() const
    {
        return Vector2{origin.x + _minX, origin.y + _minY};
    }

    inline Vector2 getMax() const
    {
        return Vector2{origin.x + _maxX, origin.y + _maxY};
    }

    Collider(Vector2 origin, float width, float height)
        : origin(origin),
          halfWidth(width / 2.0f),
          halfHeight(height / 2.0f),
          _minX(-halfWidth),
          _minY(-halfHeight),
          _maxX(halfWidth),
          _maxY(halfHeight) 
    {

    }
};