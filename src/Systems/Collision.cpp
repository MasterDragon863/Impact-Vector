#pragma once

#include "raylib.h"
#include "../Components/Components.h"
#include "../ECS.h"
#include <iostream>

struct SweepEntities
{
    struct Entry
    {
        Entity id{INVALID_ENTITY};
        Collider *collider{nullptr};
    };

    std::array<Entry, MAX_ENTITIES> entries;
    int nextAvailableSlot{0};
    std::array<int, MAX_ENTITIES> sparse{-1};


    void sort()
    {
        for (int i{1}; i < nextAvailableSlot; ++i)
        {
            Entry key = entries[i];
            if (key.collider == nullptr)
            {
                continue;
            }
            int j{i - 1};

            for (int k = 0; k < MAX_ENTITIES; ++k)
            {
                if (j < 0)
                {
                    break;
                }

                if (entries[j].collider && entries[j].collider->getMin().x > key.collider->getMin().x)
                {
                    entries[j + 1] = entries[j];
                    sparse[entries[j].id] = j + 1;
                    j--;
                }
                else
                {
                    break;
                }
            }
            entries[j + 1] = key;
            sparse[key.id] = j + 1;
        }
    }

    bool add(Entity id, Collider *col)
    {
        if (nextAvailableSlot >= MAX_ENTITIES || id >= MAX_ENTITIES)
        {
            return false;
        }

        if (sparse[id] != -1)
        {
            entries[sparse[id]].collider = col;
            sort();
            return true;
        }

        entries[nextAvailableSlot] = {id, col};
        sparse[id] = nextAvailableSlot;
        nextAvailableSlot++;

        sort();
        return true;
    }

    void remove(const Entity id)
    {
        if (id >= MAX_ENTITIES || sparse[id] == -1)
        {
            return;
        }

        int indexToRemove = sparse[id];
        int lastIndex = nextAvailableSlot - 1;

        Entry lastEntry = entries[lastIndex];
        entries[indexToRemove] = lastEntry;

        sparse[lastEntry.id] = indexToRemove;
        sparse[id] = -1;
        nextAvailableSlot--;

        sort();
    }
};

class CollisionSystem
{
public:
    static void handleCollisions(ECS &ecs)
    {
        const std::size_t totalColliders{ecs.getComponentCount<Collider>()};
        for (std::size_t i{0}; i < totalColliders; ++i)
        {
            Entity entity{ecs.getEntityFromDenseIndex<Collider>(i)};
            Collider *col{&(ecs.getComponent<Collider>(entity))};
            _sortArr.add(entity, col);
        }
        _sortArr.sort();
        std::cout << "[";
        for (int i{0}; i < MAX_ENTITIES; ++i)
        {
            if (_sortArr.entries[i].collider == nullptr)
                continue;
            std::cout << "SweepEntity(ID: " << _sortArr.entries[i].id
                      << ", MinX: " << _sortArr.entries[i].collider->getMin().x << "), ";
        }
        std::cout << "]" << std::endl
                  << std::endl;
    }

private:
    inline static SweepEntities _sortArr;

    bool _detectCollision(const Collider &objA, const Collider &objB) const
    {
        bool xCol{objA.getMin().x < objB.getMax().x && objA.getMax().x > objB.getMin().x};
        bool yCol{objA.getMin().y < objB.getMax().y && objA.getMax().y > objB.getMin().y};

        return xCol && yCol;
    }
};