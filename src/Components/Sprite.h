#pragma once
#include "raylib.h"

struct Sprite
{
    Texture2D *atlas;
    Rectangle sourceRect;
    Color debugColor;
    bool useAtlas;

    Sprite(float width, float height, Color color)
    {
        atlas = nullptr;
        sourceRect = {0.0f, 0.0f, width, height};
        debugColor = color;
        useAtlas = false;
    }

    Sprite(Texture2D *globalAtlas, Rectangle frameInAtlas)
    {
        atlas = globalAtlas;
        sourceRect = frameInAtlas;
        debugColor = WHITE;
        useAtlas = true;
    }

    Sprite()
    {
        atlas = nullptr;
        sourceRect = {0.0f, 0.0f, 0.0f, 0.0f};
        debugColor = MAGENTA;
        useAtlas = false;
    }
};