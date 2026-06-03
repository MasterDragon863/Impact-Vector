#pragma once
class Game
{
public:
    Game();
    void run();

private:
    void update();
    void render();
    bool isRunning;
};