#include "raylib.h"

int main(void)
{
    InitWindow(640, 360, "Impact Vector");

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(BLACK);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}