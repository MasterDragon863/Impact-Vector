#include "raylib.h"
#include "../Registry.h"
#include "../ECS.h"
#include "../Components/Components.h"

inline void InputSystem(Registry &registry)
{
    Velocity *velocities{registry.getComponentArray<Velocity>()};
    Input *inputs{registry.getComponentArray<Input>()};
    for (Entity entity{0}; entity < registry.getEntityCount() && entity < registry.getMaxEntities(); ++entity)
    {
        std::uint32_t bitMask{ComponentBit::INPUT | ComponentBit::VELOCITY};
        if (registry.EntityHasComponent(entity, bitMask))
        {
            Input &input{inputs[entity]};
            Velocity &velocity{velocities[entity]};

            input.addInput(
                IsKeyDown(KEY_RIGHT),
                IsKeyDown(KEY_LEFT),
                IsKeyDown(KEY_X),
                IsKeyDown(KEY_C));

            velocity.dx = (input.right - input.left) * velocity.speed;
        }
    }
}

class InputDetectionSystem
{
public:
    static void detectInput(ECS &ecs)
    {
        const std::size_t totalInputs{ecs.getComponentCount<Input>()};

        for (std::size_t i{0}; i < totalInputs; ++i)
        {
            Entity entity{ecs.getEntityFromDenseIndex<Input>(i)};

            if (ecs.hasComponent<Velocity>(entity)){
                Input &input {ecs.getComponent<Input>(entity)};
                Velocity &vel {ecs.getComponent<Velocity>(entity)};

            input.addInput(
                IsKeyDown(KEY_RIGHT),
                IsKeyDown(KEY_LEFT),
                IsKeyDown(KEY_X),
                IsKeyDown(KEY_C));

            vel.dx = (input.right - input.left) * vel.speed;
            }
        }
    }
};