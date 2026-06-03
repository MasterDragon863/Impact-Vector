#include "Game.h"
#include "raylib.h"

Game::Game()
{
    isRunning = true;
}

void Game::run()
{
    InitWindow(640, 360, "Impact Vector");

    while (isRunning)
    {
        update();
        render();
    }

    CloseWindow();
}

void Game::update()
{
    if (WindowShouldClose())
    {
        isRunning = false;
    }
}

void Game::render()
{
    BeginDrawing();
        ClearBackground(BLACK);
    EndDrawing();
}