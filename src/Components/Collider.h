#pragma once
#include "raylib.h"

enum class ColliderType : char {
  STATIC,
  DYNAMIC,
};

/**
 * @class Collider
 * @brief AABB Collider Component
 *
 * origin - Worldspace Vector 2 of the position of the object
 * halfWidth - The half wcidth the collider
 * halfHeight - The half height of the collider
 * minX - the position of minimum X relative to the origin - private
 * maxX - the position of maximum X relative to the origin - private
 * minY - the position of minimum Y relative to the origin - private
 * maxY - the position of maximum Y relative to the origin - private
 *
 * @size 36 bytes
 */
struct Collider {
  Vector2 origin{0.0f, 0.0f};
  float halfWidth{0.0f};
  float halfHeight{0.0f};
  float _minX{0.0f};
  float _minY{0.0f};
  float _maxX{0.0f};
  float _maxY{0.0f};
  ColliderType type;

  inline const Vector2 getMin() const {
    return Vector2{origin.x + _minX, origin.y + _minY};
  }

  inline const Vector2 getMax() const {
    return Vector2{origin.x + _maxX, origin.y + _maxY};
  }

  inline void updateOrigin(const float& x, const float& y) { origin = {x, y}; }

  Collider(Vector2 origin, float width, float height,
           ColliderType colType = ColliderType::STATIC)
      : origin(origin),
        halfWidth(width / 2.0f),
        halfHeight(height / 2.0f),
        _minX(halfWidth * -1),
        _minY(halfHeight * -1),
        _maxX(halfWidth),
        _maxY(halfHeight),
        type(colType) {}
  Collider() = default;
};
