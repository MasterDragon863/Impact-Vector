#include "Game.h"
#include "raylib.h"

Game::Game()
{
    _isRunning = true;

}

void Game::run()
{
    InitWindow(640, 360, "Impact Vector");
    _initializeEntities();

    while (_isRunning)
    {
        _update();
        Render(_registry);
    }

    CloseWindow();
}

void Game::_update()
{
    if (WindowShouldClose())
    {
        _isRunning = false;
    }
}

void Game::_initializeEntities()
{
    _registry.addComponent<Position>(_playerEntity, ComponentBit::POSITION, {100.0f, 100.0f});
    _registry.addComponent<Sprite>(_playerEntity, ComponentBit::SPRITE, Sprite(32.0f, 32.0f, ORANGE));
}