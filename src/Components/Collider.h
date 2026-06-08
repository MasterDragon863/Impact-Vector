#pragma once
#include "raylib.h"

struct Collider
{
    Vector2 origin{0.0f, 0.0f};
    float halfWidth{0.0f};
    float halfHeight{0.0f};
    float _minX{0.0f};
    float _minY{0.0f};
    float _maxX{0.0f};
    float _maxY{0.0f};

    inline Vector2 getMin() const
    {
        return Vector2{origin.x + _minX, origin.y + _minY};
    }

    inline Vector2 getMax() const
    {
        return Vector2{origin.x + _maxX, origin.y + _maxY};
    }

    inline void updateOrigin(const float &x, const float &y)
    {
        origin = {x, y};
    }

    Collider(Vector2 origin, float width, float height)
        : origin(origin),
          halfWidth(width / 2.0f),
          halfHeight(height / 2.0f),
          _minX(halfWidth * -1),
          _minY(halfHeight * -1),
          _maxX(halfWidth),
          _maxY(halfHeight)
    {
    }
    Collider() = default;
};