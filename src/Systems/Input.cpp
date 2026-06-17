#include "../Components/Components.h"
#include "../ECS.h"
#include "raylib.h"

/**
 * @class InputDetectionSystem
 * @brief Static Class for Input Detection
 *
 */
class InputDetectionSystem {
 public:
  /**
   * @brief Gets input and applies it to the component
   *
   * Loops through number of input components in the ecs
   * Gets the Input and Velocity Component
   * Checks if the keys are pressed down and updates velocity appropriatly
   *
   * @note - only applies to velocity if the entity has it
   *
   * @param ecs - Pass by Reference to the parent ecs
   */
  static void detectInput(ECS& ecs) {
    const std::size_t totalInputs{ecs.getComponentCount<Input>()};

    for (std::size_t i{0}; i < totalInputs; ++i) {
      Entity entity{ecs.getEntityFromDenseIndex<Input>(i)};

      if (ecs.hasComponent<Velocity>(entity)) {
        Input& input{ecs.getComponent<Input>(entity)};
        Velocity& vel{ecs.getComponent<Velocity>(entity)};

        input.addInput(IsKeyDown(KEY_RIGHT), IsKeyDown(KEY_LEFT),
                       IsKeyDown(KEY_X), IsKeyDown(KEY_C));

        vel.dx = (input.right - input.left) * vel.speed;
      }
    }
  }
};
