#pragma once
#include "raylib.h"

struct Sprite
{
    Texture2D *atlas;
    Rectangle sourceRect;
    Color debugColor;
    Vector2 origin;
    float rotation;
    bool useAtlas;

    Sprite(float width, float height, Color color, Vector2 origin, float rot)
        : atlas(nullptr),
          sourceRect{0.0f, 0.0f, width, height},
          debugColor(color),
          origin(origin),
          rotation(rot),
          useAtlas(false)
    {
    }

    Sprite(Texture2D *globalAtlas, Rectangle frameInAtlas, Vector2 origin = {0.0f, 0.0f}, float rot = 0.0f)
        : atlas(globalAtlas),
          sourceRect(frameInAtlas),
          debugColor(WHITE),
          origin(origin),
          rotation(rot),
          useAtlas(true)
    {
    }

    Sprite()
        : atlas(nullptr),
          sourceRect{0.0f, 0.0f, 0.0f, 0.0f},
          debugColor(MAGENTA),
          origin{0.0f, 0.0f},
          rotation(0.0f),
          useAtlas(false)
    {
    }
};
