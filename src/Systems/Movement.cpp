#include "raylib.h"
#include "../Registry.h"
#include "../Components/Components.h"

inline void MovementSystem(Registry &registry)
{
    for (Entity entity = 0; entity < registry.getEntityCount() && entity < registry.getMaxEntities(); ++entity)
    {
        std::uint32_t bitMask = ComponentBit::POSITION | ComponentBit::VELOCITY;
        if (registry.EntityHasComponent(entity, bitMask))
        {
            Position &position = registry.getComponentArray<Position>()[entity];
            Velocity &velocity = registry.getComponentArray<Velocity>()[entity];

            position.x += velocity.dx * velocity.speed * GetFrameTime();
            position.y += velocity.dy * velocity.speed * GetFrameTime();
        }
    }
}