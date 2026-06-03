#include "Registry.h"

Entity Registry::createEntity()
{
    if (nextEntity >= MAX_ENTITIES)
    {
        return INVALID_ENTITY;
    }
    Entity id = nextEntity;
    nextEntity++;
    return id;
}
