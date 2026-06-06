#include "raylib.h"
#include "../Components/Components.h"
#include "../ECS.h"
#include <cmath>

class PhysicsSystem
{

public:
    static void applyPhysics(ECS &ecs)
    {
        const std::size_t totalPhysicsComponents{ecs.getComponentCount<Physics>()};

        for (std::size_t i{0}; i < totalPhysicsComponents; ++i)
        {
            Entity entity{ecs.getEntityFromDenseIndex<Physics>(i)};

            if (ecs.hasComponent<Velocity>(entity))
            {
                Physics &physics{ecs.getComponent<Physics>(entity)};
                Velocity &vel{ecs.getComponent<Velocity>(entity)};

                float terminal_velocity{1000.0f};
                if (physics.friction > 0.0f)
                {
                    terminal_velocity = physics.mass * physics.gravity / physics.friction;
                }

                if (vel.dy < terminal_velocity)
                {
                    vel.dy += physics.gravity * GetFrameTime();
                }
                else
                {
                    vel.dy -= physics.friction * GetFrameTime();
                }
            }
        }
    }
};
