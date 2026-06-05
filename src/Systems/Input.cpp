#include "raylib.h"
#include "../Registry.h"
#include "../Components/Components.h"

inline void InputSystem(Registry &registry)
{
    for (Entity entity{0}; entity < registry.getEntityCount() && entity < registry.getMaxEntities(); ++entity)
    {
        std::uint32_t bitMask {ComponentBit::INPUT | ComponentBit:: VELOCITY};
        if (registry.EntityHasComponent(entity, bitMask))
        {
            Input &input {registry.getComponentArray<Input>()[entity]};
            Velocity &velocity {registry.getComponentArray<Velocity>()[entity]};
            input.addInput(
                IsKeyDown(KEY_RIGHT),
                IsKeyDown(KEY_LEFT),
                IsKeyDown(KEY_X),
                IsKeyDown(KEY_C));

            velocity.dx = (input.right - input.left) * velocity.speed;
        }

    }
}