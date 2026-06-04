#pragma once

#include "raylib.h"

#include "Registry.h"
#include "Systems/Render.cpp"
#include "Systems/Input.cpp"
class Game
{
public:
    Game();
    void run();

private:
    void _update();
    bool _isRunning;

    void _initializeEntities();

    Registry _registry;
    
    Entity _playerEntity{_registry.createEntity()};

};