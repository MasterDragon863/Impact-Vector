#include "raylib.h"
#include "../Components/Components.h"
#include "../ECS.h"


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