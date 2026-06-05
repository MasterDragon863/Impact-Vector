#include "raylib.h"
#include "../Registry.h"
#include "../Components/Components.h"
#include <cmath>

inline void PhysicsSystem(Registry &registry)
{
    Physics *physics_components {registry.getComponentArray<Physics>()};
    Velocity *velocities {registry.getComponentArray<Velocity>()};
    for (Entity entity{0}; entity < registry.getEntityCount() && entity < registry.getMaxEntities(); ++entity)
    {
        std::uint32_t bitMask {ComponentBit::PHYSICS | ComponentBit::VELOCITY};
        if (registry.EntityHasComponent(entity, bitMask))
        {
            Physics &physics {physics_components[entity]};
            Velocity &velocity {velocities[entity]}; 

            float terminal_velocity{1000.0f};
            if (physics.friction > 0.0f)
            {
                terminal_velocity = physics.mass * physics.gravity / physics.friction;
            }


            if (velocity.dy < terminal_velocity)
            {
                velocity.dy += physics.gravity * GetFrameTime();

            }
            else {
                velocity.dy -= physics.friction * GetFrameTime();
            }
        }
    }
}