#pragma once

#include <cassert>
#include <cstdio>
#include <cstdlib>

#include "../Components/Components.h"
#include "../ECS.h"
#include "Collider.h"
#include "Position.h"
#include "raylib.h"

/**
 * @class SweepEntities
 * @brief Contains sorted entires of entities with colliders and positions
 *
 * Uses Insertion sort to sort entities based on the minX value of the collider
 *
 * @size 288 bytes
 */
struct SweepEntities {
  /**
   * @class Entry
   * @brief struct that contains and entity id and pointers to collider and
   * position
   *
   * id defualts to Invalid Entity
   * Collider defualts to nullptr
   * Position defualts to nullptr
   *
   */
  struct Entry {
    Entity id{INVALID_ENTITY};
    Collider* collider{nullptr};
    Position* position{nullptr};
  };

  std::array<Entry, MAX_ENTITIES> entries;
  std::array<int, MAX_ENTITIES> sparse;
  int nextAvailableSlot{0};

  SweepEntities() { sparse.fill(-1); }

  /**
   * @brief Sorts the entries
   *
   * Using insertion sort, loops through all available entries
   * (nextAvailableSlot) Sorts based on the minumum X value of the Collider
   *
   */
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

        if (entries[j].collider &&
            entries[j].collider->getMin().x > key.collider->getMin().x) {
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

  /**
   * @brief Adds and entity to the array
   *
   * @param id - the Entity id
   * @param col - pointer to the Collider
   * @param pos - pointer to the Position
   * @return did the addition succeed
   */
  const bool add(Entity& id, Collider* col, Position* pos) {
    if (nextAvailableSlot >= MAX_ENTITIES || id >= MAX_ENTITIES) {
      return false;
    }

    if (sparse[id] != -1) {
      entries[sparse[id]].collider = col;
      sort();
      return true;
    }

    entries[nextAvailableSlot] = {.id = id, .collider = col, .position = pos};
    sparse[id] = nextAvailableSlot;
    nextAvailableSlot++;

    return true;
  }

  /**
   * @brief Removes and entity
   *
   * Removes by swapping with that last valid entry and then
   * sets the values to the defualt entry values
   *
   * @param id The enity id you want to remove
   */
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

  inline Entry getEntry(Entity entity) {
    assert(entity != INVALID_ENTITY);
    assert(entity < nextAvailableSlot);

    return entries[sparse[entity]];
  }
};

/**
 * @class Penetration
 * @brief Simple stuct that hold the penetration values
 *
 * minMaxX - Minimum X value of Collider A
 * substracted by Maximum X of Collider B
 * maxMinX - Maximum X value of Collider A
 * substracted by Minimum X of Collider B
 *
 * minMaxY - Minimum Y value of Collider A
 * substracted by Maximum Y of Collider B
 * maxMinY - Maximum Y value of Collider A
 * substracted by Minimum Y of Collider B
 *
 * @size 16 bytes
 */
struct Penetration {
  float minMaxX;
  float maxMinX;
  float minMaxY;
  float maxMinY;
};

/**
 * @class CollisionSystem
 * @brief Static System class for handling collisions
 *
 */
class CollisionSystem {
 public:
  /**
   * @brief Static function that calls the appropriate methods
   * and functions to resolve collisions
   *
   * Adds the entries from the ecs
   * Sorts the sweep line array
   * Resolves the collisions
   *
   * @param ecs - Reference to the parent ecs
   */
  static void handleCollisions(ECS& ecs) {
    _addEntries(ecs);
    _sortArr.sort();
    _resolveCollision(ecs);
  }

 private:
  inline static SweepEntities _sortArr;

  /**
   * @brief Detects the Collisions based on AABB
   *
   * @param objA - Collider 1
   * @param objB - Collider 2
   * @return - true if collision occured, else false
   */
  inline static bool _detectCollision(const Collider* objA,
                                      const Collider* objB) {
    bool xCol{objA->getMin().x < objB->getMax().x &&
              objA->getMax().x > objB->getMin().x};
    bool yCol{objA->getMin().y < objB->getMax().y &&
              objA->getMax().y > objB->getMin().y};

    return xCol && yCol;
  }

  /**
   * @brief Adds the entries to _sortArr
   *
   * Loops through every collider and adds it.
   * @note - asserts that every entity with a collider must have a position
   *
   * @param ecs - REference to the ecs component
   */
  inline static void _addEntries(ECS& ecs) {
    const std::size_t totalColliders{ecs.getComponentCount<Collider>()};
    for (std::size_t i{0}; i < totalColliders; ++i) {
      Entity entity{ecs.getEntityFromDenseIndex<Collider>(i)};

      assert(ecs.hasComponent<Position>(entity));

      Collider* col{&(ecs.getComponent<Collider>(entity))};
      Position* pos{&(ecs.getComponent<Position>(entity))};

      (void)_sortArr.add(entity, col, pos);
    }
  }

  /**
   * @brief Function That handles the detection and resolutio of collisions
   *
   * Loops through every entry in the collider
   * If the entry is out of bounds of the current collider breaks out of the
   * loop If collision is detected hands it of to the appropriate function based
   * on collider type
   *
   * @param ecs - Reference to the parent ecs
   */
  inline static void _resolveCollision(ECS& ecs) {
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
                entryA.collider->type == ColliderType::DYNAMIC ? entryA
                                                               : entryB};
            SweepEntities::Entry staticEntry{
                entryA.collider->type == ColliderType::STATIC ? entryA
                                                              : entryB};

            resolveDynamicStaticCollisions(dynamicEntry, staticEntry, ecs);
          }
        }
      }
    }
  }

  /**
   * @brief Resolves Collisions bewteen a Dynamic and Static Collider
   *
   * Calculates the Pentration.
   * Gets the smallest penetration value and substracts that from the
   * dynamic entries position.
   * if the entry has a velocity, set it to 0;
   *
   * @param dynamicEntry - Entry with the Dynamic Collider
   * @param staticEntry - Entriy with the Static Collider
   * @param ecs - Reference to the ecs
   */
  inline static void resolveDynamicStaticCollisions(
      SweepEntities::Entry& dynamicEntry, SweepEntities::Entry& staticEntry,
      ECS& ecs) {
    Penetration penetration = {
        dynamicEntry.collider->getMin().x - staticEntry.collider->getMax().x,
        dynamicEntry.collider->getMax().x - staticEntry.collider->getMin().x,
        dynamicEntry.collider->getMin().y - staticEntry.collider->getMax().y,
        dynamicEntry.collider->getMax().y - staticEntry.collider->getMin().y,
    };

    float overlapX{
        (std::abs(penetration.minMaxX) < std::abs(penetration.maxMinX))
            ? penetration.minMaxX
            : penetration.maxMinX};
    float overlapY{
        (std::abs(penetration.minMaxY) < std::abs(penetration.maxMinY))
            ? penetration.minMaxY
            : penetration.maxMinY};

    if (std::abs(overlapX) < std::abs(overlapY)) {
      dynamicEntry.position->x -= overlapX;

      if (ecs.hasComponent<Velocity>(dynamicEntry.id)) {
        ecs.getComponent<Velocity>(dynamicEntry.id).dx = 0;
      }
    } else {
      dynamicEntry.position->y -= overlapY;
      if (ecs.hasComponent<Velocity>(dynamicEntry.id)) {
        ecs.getComponent<Velocity>(dynamicEntry.id).dy = 0;
      }
    }
  }
};
