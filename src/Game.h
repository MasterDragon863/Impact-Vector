#pragma once

#include "raylib.h"

#include "Registry.h"
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

    void _initializeEntities();

    Registry _registry;
    
    Entity _playerEntity{_registry.createEntity()};

};