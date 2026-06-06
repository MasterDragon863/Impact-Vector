#include "Game.h"


Game::Game():
    _isRunning(true)
{
    SetTargetFPS(60);
    _playerEntity = _registry.createEntity();
    _player = _ecs.createEntity();
}

void Game::run()
{
    InitWindow(_virtualWidth, _virtualHeight, "Impact Vector");
    ToggleBorderlessWindowed();
    SetWindowFocused();

    _target = LoadRenderTexture(_virtualWidth, _virtualHeight);
    SetTextureFilter(_target.texture, TEXTURE_FILTER_POINT);

    _initializeEntities();

    while (_isRunning)
    {
        _update();
        // InputSystem(_registry);
        PhysicsSystem(_registry);
        // movement(_registry);
        Render(_registry, _target);
        Renderer::draw(_ecs, _target);
        InputDetectionSystem::detectInput(_ecs);
        MovementSystem::moveEntities(_ecs);
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
    _ecs.addComponent<Input>(_player, Input{});
    _ecs.addComponent<Velocity>(_player, Velocity{0.0f, 0.0f, 250.0f});

}