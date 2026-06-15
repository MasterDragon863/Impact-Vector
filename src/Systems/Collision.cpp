#pragma once

#include "../Components/Components.h"
#include "../ECS.h"
#include "Collider.h"
#include "Position.h"
#include "raylib.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>

struct SweepEntities {
    struct Entry {
        Entity id{INVALID_ENTITY};
        Collider *collider{nullptr};
        Position *position{nullptr};
    };

    std::array<Entry, MAX_ENTITIES> entries;
    int nextAvailableSlot{0};
    std::array<int, MAX_ENTITIES> sparse;

    SweepEntities() { sparse.fill(-1); }

    void sort() {
        for (int i{1}; i < nextAvailableSlot; ++i) {
            Entry key = entries[i];
            if (key.collider == nullptr) {
                continue;
            }
            int j{i - 1};

            for (int k = 0; k < MAX_ENTITIES; ++k) {
                if (j < 0) {
                    break;
                }

                if (entries[j].collider && entries[j].collider->getMin().x >
                                               key.collider->getMin().x) {
                    entries[j + 1] = entries[j];
                    sparse[entries[j].id] = j + 1;
                    j--;
                } else {
                    break;
                }
            }
            entries[j + 1] = key;
            sparse[key.id] = j + 1;
        }
    }

    bool add(Entity id, Collider *col, Position *pos) {
        if (nextAvailableSlot >= MAX_ENTITIES || id >= MAX_ENTITIES) {
            return false;
        }

        if (sparse[id] != -1) {
            entries[sparse[id]].collider = col;
            sort();
            return true;
        }

        entries[nextAvailableSlot] = {
            .id = id, .collider = col, .position = pos};
        sparse[id] = nextAvailableSlot;
        nextAvailableSlot++;

        return true;
    }

    void remove(const Entity id) {
        if (id >= MAX_ENTITIES || sparse[id] == -1) {
            return;
        }

        int indexToRemove = sparse[id];
        int lastIndex = nextAvailableSlot - 1;

        Entry lastEntry = entries[lastIndex];
        entries[indexToRemove] = lastEntry;

        sparse[lastEntry.id] = indexToRemove;
        sparse[id] = -1;
        nextAvailableSlot--;
    }

    inline const int getLength() const { return nextAvailableSlot; }

    inline const Collider *getCollider(const Entity &entity) const {
        if (entity == INVALID_ENTITY || entity > MAX_ENTITIES) {
            return nullptr;
        }
        return entries[sparse[entity]].collider;
    }

    inline Entry getEntry(Entity entity) {
        assert(entity != INVALID_ENTITY);
        assert(entity < nextAvailableSlot);

        return entries[sparse[entity]];
    }
};

struct Penetration {
    float minMaxX;
    float maxMinX;
    float minMaxY;
    float maxMinY;
};

class CollisionSystem {
  public:
    static void handleCollisions(ECS &ecs) {

        _addEntries(ecs);
        _sortArr.sort();
        _resolveCollision(ecs);

        /*
        std::cout << "[";
        for (std::size_t i{0}; i < MAX_ENTITIES; ++i) {
            if (_sortArr.entries[i].collider == nullptr)
                continue;
            std::cout << "SweepEntity(ID: " << _sortArr.entries[i].id
                      << ", MinX: " << _sortArr.entries[i].collider->getMin().x
                      << "), ";
        }
        std::cout << "]" << std::endl << std::endl; */
    }

  private:
    inline static SweepEntities _sortArr;

    static bool _detectCollision(const Collider *objA, const Collider *objB) {
        bool xCol{objA->getMin().x < objB->getMax().x &&
                  objA->getMax().x > objB->getMin().x};
        bool yCol{objA->getMin().y < objB->getMax().y &&
                  objA->getMax().y > objB->getMin().y};

        return xCol && yCol;
    }

    inline static void _addEntries(ECS &ecs) {

        const std::size_t totalColliders{ecs.getComponentCount<Collider>()};
        for (std::size_t i{0}; i < totalColliders; ++i) {
            Entity entity{ecs.getEntityFromDenseIndex<Collider>(i)};
            assert(ecs.hasComponent<Position>(entity));
            Collider *col{&(ecs.getComponent<Collider>(entity))};
            Position *pos{&(ecs.getComponent<Position>(entity))};

            (void)_sortArr.add(entity, col, pos);
        }
    }

    inline static void _resolveCollision(ECS &ecs) {
        for (std::size_t i{0}; i < _sortArr.getLength() - 1; ++i) {
            for (std::size_t j{i + 1}; j < _sortArr.getLength(); ++j) {
                SweepEntities::Entry entryA{_sortArr.getEntry(i)};
                SweepEntities::Entry entryB{_sortArr.getEntry(j)};

                if (entryB.collider->getMin().x > entryA.collider->getMax().x) {
                    break;
                }

                if (_detectCollision(entryA.collider, entryB.collider)) {
                    if (entryA.collider->type != entryB.collider->type) {
                        SweepEntities::Entry dynamicEntry{
                            entryA.collider->type == ColliderType::DYNAMIC
                                ? entryA
                                : entryB};
                        SweepEntities::Entry staticEntry{
                            entryA.collider->type == ColliderType::STATIC
                                ? entryA
                                : entryB};

                        Penetration penetration = {
                            dynamicEntry.collider->getMin().x -
                                staticEntry.collider->getMax().x,
                            dynamicEntry.collider->getMax().x -
                                staticEntry.collider->getMin().x,
                            dynamicEntry.collider->getMin().y -
                                staticEntry.collider->getMax().y,
                            dynamicEntry.collider->getMax().y -
                                staticEntry.collider->getMin().y,
                        };

                        float overlapX{(std::abs(penetration.minMaxX) <
                                        std::abs(penetration.maxMinX))
                                           ? penetration.minMaxX
                                           : penetration.maxMinX};
                        float overlapY{(std::abs(penetration.minMaxY) <
                                        std::abs(penetration.maxMinY))
                                           ? penetration.minMaxY
                                           : penetration.maxMinY};

                        if (std::abs(overlapX) < std::abs(overlapY)) {
                            dynamicEntry.position->x -= overlapX;

                            if (ecs.hasComponent<Velocity>(dynamicEntry.id)){
                                ecs.getComponent<Velocity>(dynamicEntry.id).dx = 0;
                            }
                        } else {
                            dynamicEntry.position->y -= overlapY;
                            if (ecs.hasComponent<Velocity>(dynamicEntry.id)){
                                ecs.getComponent<Velocity>(dynamicEntry.id).dy = 0;
                            }
                        }
                    }
                }
            }
        }
    }
};
