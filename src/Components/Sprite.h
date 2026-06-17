#pragma once
#include "raylib.h"

/**
 * @class Sprite
 * @brief Struct that handles all data related to drawing on the screen
 * 
 * @size 48 bytes
 */
struct Sprite {
  Rectangle sourceRect;
  Texture2D* atlas;
  Vector2 origin;
  Color debugColor;
  float rotation;
  bool useAtlas;

  Sprite(float width, float height, Color color, Vector2 origin, float rot)
      : sourceRect{0.0f, 0.0f, width, height},
        atlas(nullptr),
        origin(origin),
        debugColor(color),
        rotation(rot),
        useAtlas(false) {}

  Sprite(Texture2D* globalAtlas, Rectangle frameInAtlas,
         Vector2 origin = {0.0f, 0.0f}, float rot = 0.0f)
      : sourceRect(frameInAtlas),
        atlas(globalAtlas),
        origin(origin),
        debugColor(WHITE),
        rotation(rot),
        useAtlas(true) {}

  Sprite()
      : sourceRect{0.0f, 0.0f, 0.0f, 0.0f},
        atlas(nullptr),
        origin{0.0f, 0.0f},
        debugColor(MAGENTA),
        rotation(0.0f),
        useAtlas(false) {}
};
