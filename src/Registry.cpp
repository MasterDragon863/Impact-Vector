#include "Registry.h"


Registry::Registry()
{
    component_storage[POSITION_TYPE] = positions;
    component_storage[VELOCITY_TYPE] = velocities;
    component_storage[INPUT_TYPE] = inputs;
    component_storage[SPRITE_TYPE] = sprites;
    component_storage[PHYSICS_TYPE] = physics;
    component_storage[COLLIDER_TYPE] = colliders;
}



Entity Registry::createEntity()
{
    if (nextEntity >= MAX_ENTITIES)
    {
        return INVALID_ENTITY;
    }
    Entity id {nextEntity};
    nextEntity++;
    return id;
}

bool Registry::EntityHasComponent(Entity entity, std::uint32_t component) const
{
    if (entity == INVALID_ENTITY || entity >= nextEntity)
    {
        return false;
    }
    return (component_masks[entity] & component) != 0;
}
