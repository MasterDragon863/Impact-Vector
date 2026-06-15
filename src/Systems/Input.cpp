#include "../Components/Components.h"
#include "../ECS.h"
#include "raylib.h"

class InputDetectionSystem {
  public:
    static void detectInput(ECS &ecs) {
        const std::size_t totalInputs{ecs.getComponentCount<Input>()};

        for (std::size_t i{0}; i < totalInputs; ++i) {
            Entity entity{ecs.getEntityFromDenseIndex<Input>(i)};

            if (ecs.hasComponent<Velocity>(entity)) {
                Input &input{ecs.getComponent<Input>(entity)};
                Velocity &vel{ecs.getComponent<Velocity>(entity)};

                input.addInput(IsKeyDown(KEY_RIGHT), IsKeyDown(KEY_LEFT),
                               IsKeyDown(KEY_X), IsKeyDown(KEY_C));

                vel.dx = (input.right - input.left) * vel.speed;
            }
        }
    }
};