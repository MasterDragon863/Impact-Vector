
#include <cassert>
#include "../Components/Components.h"
#include "../ECS.h"
#include "raylib.h"

/**
 * @class PhysicsSystem
 * @brief Static class that applies gravity
 *
 */
class PhysicsSystem {
 public:
  /**
   * @brief static function that applies physics
   *
   * Loops through every Physics Component
   * For each physics component applies gravity
   *    @note implements a terminal velocity based on the mass
   *
   *
   * @param ecs - Reference to the parent ecs
   */
  static void applyPhysics(ECS& ecs) {
    const std::size_t totalPhysicsComponents{ecs.getComponentCount<Physics>()};

    for (std::size_t i{0}; i < totalPhysicsComponents; ++i) {
      Entity entity{ecs.getEntityFromDenseIndex<Physics>(i)};

      assert(ecs.hasComponent<Velocity>(entity));

      Physics& physics{ecs.getComponent<Physics>(entity)};
      Velocity& vel{ecs.getComponent<Velocity>(entity)};

      float terminal_velocity{1000.0f};
      if (physics.friction > 0.0f) {
        terminal_velocity = physics.mass * physics.gravity / physics.friction;
      }

      if (vel.dy < terminal_velocity) {
        vel.dy += physics.gravity * GetFrameTime();
      } else {
        vel.dy -= physics.friction * GetFrameTime();
      }
    }
  }
};
