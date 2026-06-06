#include "raylib.h"
#include "../Registry.h"
#include "../Components/Components.h"
#include "../ECS.h"

inline void movement(Registry &registry)
{
    Position *positions{registry.getComponentArray<Position>()};
    Velocity *velocities{registry.getComponentArray<Velocity>()};
    for (Entity entity{0}; entity < registry.getEntityCount() && entity < registry.getMaxEntities(); ++entity)
    {
        std::uint32_t bitMask{ComponentBit::POSITION | ComponentBit::VELOCITY};
        if (registry.EntityHasComponent(entity, bitMask))
        {
            Position &position{positions[entity]};
            Velocity &velocity{velocities[entity]};

            position.x += velocity.dx * GetFrameTime();
            position.y += velocity.dy * GetFrameTime();
        }
    }
}

class MovementSystem
{
public:
    static void moveEntities(ECS &ecs)
    {
        const std::size_t totalMovableEntites{ecs.getComponentCount<Velocity>()};
        for (std::size_t i{0}; i < totalMovableEntites; ++i)
        {
            Entity entity{ecs.getEntityFromDenseIndex<Velocity>(i)};

            if (ecs.hasComponent<Position>(entity))
            {
                Velocity &vel{ecs.getComponent<Velocity>(entity)};
                Position &pos{ecs.getComponent<Position>(entity)};

                pos.x += vel.dx * GetFrameTime();
                pos.y += vel.dy * GetFrameTime();
            }
        }
    }
};