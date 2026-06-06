#pragma once

#include "raylib.h"

#include "ECS.h"
#include "Systems/Render.cpp"
#include "Systems/Input.cpp"
#include "Systems/Movement.cpp"
#include "Systems/Physics.cpp"
class Game
{
public:
    Game();
    void run();

private:
    void _update();
    bool _isRunning{false};
    RenderTexture2D _target;
    const int _virtualWidth {640};
    const int _virtualHeight {360};

    void _initializeEntities();

    ECS _ecs;
    
    Entity _player;

};