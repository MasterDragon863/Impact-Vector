#pragma once
#include "ECS.h"

#include <cstdint>
#include <limits>

#include "Components/Components.h"



enum ComponentBit
{
    POSITION = 1 << 0,
    VELOCITY = 1 << 1,
    INPUT = 1 << 2,
    SPRITE = 1 << 3,
    PHYSICS = 1 << 4,
    COLLIDER = 1 << 5, 
};

enum ComponentType
{
    POSITION_TYPE,
    VELOCITY_TYPE,
    INPUT_TYPE,
    SPRITE_TYPE,
    PHYSICS_TYPE,
    COLLIDER_TYPE, 
};

class Registry
{
public:
    void *component_storage[6]{};

    Registry();
    Entity createEntity();

    inline Entity getEntityCount() const { return nextEntity; }

    inline Entity getMaxEntities() const { return MAX_ENTITIES; }

    bool EntityHasComponent(Entity entity, std::uint32_t component) const;

    template <typename T>
    inline void addComponent(Entity entity, ComponentBit bit, T component);

    template <typename T> 
    T *getComponentArray();

private:
    Position positions[MAX_ENTITIES]{};
    Velocity velocities[MAX_ENTITIES]{};
    Input inputs[MAX_ENTITIES]{};
    Sprite sprites[MAX_ENTITIES]{};
    Physics physics[MAX_ENTITIES]{};
    Collider colliders[MAX_ENTITIES]{};


    std::uint32_t component_masks[MAX_ENTITIES]{0};

    Entity nextEntity{0};
};

template <>
inline Position *Registry::getComponentArray<Position>()
{
    return static_cast<Position *>(component_storage[POSITION_TYPE]);
}

template <>
inline Velocity *Registry::getComponentArray<Velocity>()
{
    return static_cast<Velocity *>(component_storage[VELOCITY_TYPE]);
}

template <>
inline Input *Registry::getComponentArray<Input>()
{
    return static_cast<Input *>(component_storage[INPUT_TYPE]);
}

template <>
inline Sprite *Registry::getComponentArray<Sprite>()
{
    return static_cast<Sprite *>(component_storage[SPRITE_TYPE]);
}
template <>
inline Physics *Registry::getComponentArray<Physics>()
{
    return static_cast<Physics *>(component_storage[PHYSICS_TYPE]);
}

template <>
inline Collider *Registry::getComponentArray<Collider>(){
    return static_cast<Collider *> (component_storage[COLLIDER_TYPE]);
}


template <typename T>
inline void Registry::addComponent(Entity entity, ComponentBit bit, T component)
{
    if (entity == INVALID_ENTITY || entity >= nextEntity)
    {
        return;
    }
    T *componentArray {getComponentArray<T>()};
    componentArray[entity] = component;
    component_masks[entity] |= bit;
}