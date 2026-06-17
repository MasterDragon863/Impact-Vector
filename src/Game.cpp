#include "Game.h"

#include "Collider.h"

Game::Game() : _isRunning(true) {
  _player = _ecs.createEntity();
  _tile = _ecs.createEntity();
}

void Game::run() {
  InitWindow(_virtualWidth, _virtualHeight, "Impact Vector");
  SetTargetFPS(60);
  ToggleBorderlessWindowed();
  SetWindowFocused();

  _target = LoadRenderTexture(_virtualWidth, _virtualHeight);
  SetTextureFilter(_target.texture, TEXTURE_FILTER_POINT);

  _initializeEntities();

  while (_isRunning) {
    _update();
  }

  CloseWindow();
}

void Game::_update() {
  if (WindowShouldClose()) {
    _isRunning = false;
  }
  InputDetectionSystem::detectInput(_ecs);
  PhysicsSystem::applyPhysics(_ecs);
  MovementSystem::moveEntities(_ecs);
  CollisionSystem::handleCollisions(_ecs);
  Renderer::draw(_ecs, _target);
  Renderer::scaleToScreen(_target);
}

void Game::_initializeEntities() {
  _ecs.addComponent<Position>(_player, Position{100.0f, 100.0f});
  _ecs.addComponent<Sprite>(_player,
                            Sprite{32.0f, 32.0f, ORANGE, {16.0f, 16.0f}, 0.0f});
  _ecs.addComponent<Collider>(
      _player, {Vector2{100.0f, 100.0f}, 32.0f, 32.0f, ColliderType::DYNAMIC});
  _ecs.addComponent<Input>(_player, Input{});
  _ecs.addComponent<Velocity>(_player, Velocity{0.0f, 0.0f, 250.0f});
  _ecs.addComponent<Physics>(_player, Physics(980.0f, 0.1f, 1.0f, 0.0f));

  _ecs.addComponent<Position>(_tile, Position{100.0f, 300.0f});
  _ecs.addComponent<Sprite>(_tile,
                            Sprite{32.0f, 32.0f, RED, {16.0f, 16.0f}, 0.0f});
  _ecs.addComponent<Collider>(_tile, {Vector2{100.0f, 300.0f}, 32.0f, 32.0f});
}
