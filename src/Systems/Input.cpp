#include "raylib.h"
#include "../Registry.h"
#include "../Components/Components.h"

inline void InputSystem(Registry &registry)
{
    for (Entity entity = 0; entity < registry.getEntityCount() && entity < registry.getMaxEntities(); ++entity)
    {
        std::uint32_t bitMask = ComponentBit::INPUT;
        if (registry.EntityHasComponent(entity, bitMask))
        {
            Input &input = registry.getComponentArray<Input>()[entity];
            input.addInput(
                IsKeyDown(KEY_D),
                IsKeyDown(KEY_A),
                IsKeyDown(KEY_SPACE),
                IsKeyDown(KEY_LEFT_SHIFT));
        }
    }
}