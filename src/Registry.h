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
    INPUT = 1 << 2
};

enum ComponentType
{
    POSITION_TYPE,
    VELOCITY_TYPE,
    INPUT_TYPE
};

class Registry
{
public:
    void *component_storage[3]{};

    Registry();
    Entity createEntity();

    inline Entity getEntityCount() const { return nextEntity; }

    inline Entity getMaxEntities() const { return MAX_ENTITIES; }

    bool EntityHasComponent(Entity entity, ComponentBit component) const;

    template <typename T>
    inline void addComponent(Entity entity, ComponentBit bit, T component);

    template <typename T>
    T *getComponentArray();

private:
    Position positions[MAX_ENTITIES]{};
    Velocity velocities[MAX_ENTITIES]{};
    Input inputs[MAX_ENTITIES]{};

    std::uint8_t component_masks[MAX_ENTITIES]{};

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