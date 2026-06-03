#pragma once

#include <cstdint>
#include <stdexcept>
#include <limits>

using Entity = std::uint32_t;
const Entity MAX_ENTITIES{1000};
const Entity INVALID_ENTITY{std::numeric_limits<Entity>::max()};

class Registry
{
public:
    Entity createEntity();

    inline Entity getEntityCount() const { return nextEntity; }

    inline Entity getMaxEntities() const { return MAX_ENTITIES; }

private:
    Entity nextEntity{0};

    void *component_storage[0]{};
};