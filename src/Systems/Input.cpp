#include "raylib.h"
#include "../Registry.h"

#include "../Components/Components.h"

inline void InputSystem(Registry &registry)
{
    Velocity *velocities {registry.getComponentArray<Velocity>()};
    Input *inputs {registry.getComponentArray<Input>()};
    for (Entity entity{0}; entity < registry.getEntityCount() && entity < registry.getMaxEntities(); ++entity)
    {
        std::uint32_t bitMask {ComponentBit::INPUT | ComponentBit:: VELOCITY};
        if (registry.EntityHasComponent(entity, bitMask))
        {
            Input &input {inputs[entity]};
            Velocity &velocity {velocities[entity]};

            input.addInput(
                IsKeyDown(KEY_RIGHT),
                IsKeyDown(KEY_LEFT),
                IsKeyDown(KEY_X),
                IsKeyDown(KEY_C));

            velocity.dx = (input.right - input.left) * velocity.speed;
        }

    }
}