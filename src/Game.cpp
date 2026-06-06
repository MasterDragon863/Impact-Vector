#include "Game.h"


Game::Game():
    _isRunning(true)
{
    SetTargetFPS(60);
}

void Game::run()
{
    InitWindow(640, 360, "Impact Vector");
    _initializeEntities();

    while (_isRunning)
    {
        _update();
        InputSystem(_registry);
        PhysicsSystem(_registry);
        MovementSystem(_registry);
        Render(_registry);
        Renderer::draw(_ecs);
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
    _registry.addComponent<Sprite>(_playerEntity, ComponentBit::SPRITE, Sprite(32.0f, 32.0f, ORANGE, Vector2(16.0f, 16.0f), 0.0f));
    _registry.addComponent<Input>(_playerEntity, ComponentBit::INPUT, Input());
    _registry.addComponent<Velocity>(_playerEntity, ComponentBit::VELOCITY, {0.0f, 0.0f, 250.0f});
    _registry.addComponent<Physics>(_playerEntity, ComponentBit::PHYSICS, Physics(980.0f, 0.1f, 1.0f, 0.0f));

    _ecs.addComponent<Position>(_player, Position{300.0f,300.0f});
    _ecs.addComponent<Sprite>(_player, Sprite{32.0f, 32.0f, ORANGE, {32.0f, 32.0f}, 0.0f});
}