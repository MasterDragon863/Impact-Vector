#include "ECS.h"

Entity ECS::createEntity() {
  if (nextEntity >= MAX_ENTITIES) {
    return INVALID_ENTITY;
  }
  Entity id = nextEntity;
  nextEntity++;
  return id;
}

void ECS::destroyEntity(Entity entity) {
  if (entity == INVALID_ENTITY || entity >= nextEntity) {
    return;
  }

  for (const std::unique_ptr<IComponentPool>& pool : pools) {
    if (pool != nullptr) {
      pool->remove(entity);
    }
  }
}
