#include "../Components/Components.h"
#include "../ECS.h"
#include "raylib.h"

/**
 * @class MovementSystem
 * @brief Static class with one function to handle the movement on Entities
 *
 */
class MovementSystem {
 public:
  /**
   * @brief Applies velocity to position.
   *
   * Time Complexity: O(n)
   *
   * Loops through all entities with a velocity component.
   * Checks if said entity has Position component.
   * Applies velocity to the Position by simple addition.
   *
   * If said entity also has a collider component:
   * - Update the origin of the collider to be the position of the entity.
   *
   * @param ecs - Direct reference to the ecs managing all the components.
   */
  static void moveEntities(ECS& ecs) {
    const std::size_t totalMovableEntites{ecs.getComponentCount<Velocity>()};
    for (std::size_t i{0}; i < totalMovableEntites; ++i) {
      Entity entity{ecs.getEntityFromDenseIndex<Velocity>(i)};

      if (ecs.hasComponent<Position>(entity)) {
        Velocity& vel{ecs.getComponent<Velocity>(entity)};
        Position& pos{ecs.getComponent<Position>(entity)};

        pos.x += vel.dx * GetFrameTime();
        pos.y += vel.dy * GetFrameTime();

        if (ecs.hasComponent<Collider>(entity)) {
          Collider& col{ecs.getComponent<Collider>(entity)};

          col.updateOrigin(pos.x, pos.y);
        }
      }
    }
  }
};
