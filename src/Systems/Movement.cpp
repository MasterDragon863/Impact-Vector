#include "raylib.h"
#include "../Registry.h"
#include "../Components/Components.h"

inline void MovementSystem(Registry &registry)
{
    Position *positions {registry.getComponentArray<Position>()};
    Velocity *velocities {registry.getComponentArray<Velocity>()};
    for (Entity entity {0}; entity < registry.getEntityCount() && entity < registry.getMaxEntities(); ++entity)
    {
        std::uint32_t bitMask {ComponentBit::POSITION | ComponentBit::VELOCITY};
        if (registry.EntityHasComponent(entity, bitMask))
        {
            Position &position {positions[entity]};
            Velocity &velocity {velocities[entity]}; 

            position.x += velocity.dx * GetFrameTime();
            position.y += velocity.dy * GetFrameTime();
        }
    }
}