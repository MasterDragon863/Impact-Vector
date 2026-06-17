#pragma once

#include "ECS.h"
#include "Systems/Collision.cpp"
#include "Systems/Input.cpp"
#include "Systems/Movement.cpp"
#include "Systems/Physics.cpp"
#include "Systems/Render.cpp"
#include "raylib.h"

class Game {
 public:
  Game();
  /**
   * @brief The Main Function that runs the game.
   */
  void run();

 private:
  /**
   * @brief the function that runs every frame and calls all the systems
   */
  void _update();
  bool _isRunning{false};
  RenderTexture2D _target;
  const int _virtualWidth{640};
  const int _virtualHeight{360};

  /**
   * @brief adds the components for each and every entity
   */
  void _initializeEntities();

  ECS _ecs;

  Entity _tile;
  Entity _player;
};
