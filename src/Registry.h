#pragma once

#include <cstdint>
#include <limits>

#include "Components/Components.h"

using Entity = std::uint32_t;
const Entity MAX_ENTITIES{1000};
const Entity INVALID_ENTITY{std::numeric_limits<Entity>::max()};

enum ComponentBit
{
    POSITION = 1 << 0,
    VELOCITY = 1 << 1,
    INPUT = 1 << 2,
    SPRITE = 1 << 3,
};

enum ComponentType
{
    POSITION_TYPE,
    VELOCITY_TYPE,
    INPUT_TYPE,
    SPRITE_TYPE,
};

class Registry
{
public:
    void *component_storage[4]{};

    Registry();
    Entity createEntity();

    inline Entity getEntityCount() const { return nextEntity; }

    inline Entity getMaxEntities() const { return MAX_ENTITIES; }

    bool EntityHasComponent(Entity entity, std::uint32_t component) const;

    template <typename T>
    inline void addComponent(Entity entity, ComponentBit bit, T component);

    template <typename T> 
    T *getComponentArray() const;

private:
    Position positions[MAX_ENTITIES]{};
    Velocity velocities[MAX_ENTITIES]{};
    Input inputs[MAX_ENTITIES]{};
    Sprite sprites[MAX_ENTITIES]{};

    std::uint32_t component_masks[MAX_ENTITIES]{0};

    Entity nextEntity{0};
};

template <>
inline Position *Registry::getComponentArray<Position>() const
{
    return static_cast<Position *>(component_storage[POSITION_TYPE]);
}

template <>
inline Velocity *Registry::getComponentArray<Velocity>() const
{
    return static_cast<Velocity *>(component_storage[VELOCITY_TYPE]);
}

template <>
inline Input *Registry::getComponentArray<Input>() const
{
    return static_cast<Input *>(component_storage[INPUT_TYPE]);
}

template <>
inline Sprite *Registry::getComponentArray<Sprite>() const
{
    return static_cast<Sprite *>(component_storage[SPRITE_TYPE]);
}


template <typename T>
inline void Registry::addComponent(Entity entity, ComponentBit bit, T component)
{
    if (entity == INVALID_ENTITY || entity >= nextEntity)
    {
        return;
    }
    T *componentArray = getComponentArray<T>();
    componentArray[entity] = component;
    component_masks[entity] |= bit;
}