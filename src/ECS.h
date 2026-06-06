#pragma once

#include <cstdint>
#include <limits>
#include <memory>
#include <array>

using Entity = std::uint32_t;
constexpr Entity MAX_ENTITIES{1000};
constexpr Entity INVALID_ENTITY{std::numeric_limits<Entity>::max()};
constexpr std::size_t MAX_COMPONENT_TYPES{32};

struct ComponentIdCounter
{
    inline static std::uint32_t nextTypeId{0};

    template <typename T>
    static std::uint32_t getId()
    {
        static const std::uint32_t id = nextTypeId++;
        return id;
    }
};

class IComponentPool
{
public:
    virtual ~IComponentPool() = default;
    virtual void remove(Entity entity) = 0;
    virtual bool has(Entity entity) const = 0;
};

template <typename T, std::size_t Capacity>
class ComponentPool : public IComponentPool
{
public:
    ComponentPool()
    {
        sparse_to_dense.fill(std::numeric_limits<std::size_t>::max());
    }

    ~ComponentPool() override = default;

    bool add(Entity entity, const T& component)
    {
        if (entity >= Capacity || has(entity) || num_components >= Capacity)
        {
            return false;
        }

        sparse_to_dense[entity] = num_components;
        dense_to_entity[num_components] = entity;
        dense_components[num_components] = component;

        num_components++;
        return true;
    }

    T& get(Entity entity)
    {
        std::size_t dense_index = sparse_to_dense[entity];
        return dense_components[dense_index];
    }

    bool has(Entity entity) const override
    {
        if (entity >= Capacity)
            return false;
        return sparse_to_dense[entity] != std::numeric_limits<std::size_t>::max();
    }

    void remove(Entity entity) override
    {
        if (!has(entity))
            return;

        std::size_t index_to_remove = sparse_to_dense[entity];
        std::size_t last_index = num_components - 1;

        dense_components[index_to_remove] = dense_components[last_index];

        Entity last_entity = dense_to_entity[last_index];
        dense_to_entity[index_to_remove] = last_entity;

        sparse_to_dense[last_entity] = index_to_remove;
        sparse_to_dense[entity] = std::numeric_limits<std::size_t>::max();

        num_components--;
    }

    std::size_t size() const { return num_components; }

    Entity getEntityAtDenseIndex(std::size_t index) const
    {
        return dense_to_entity[index];
    }

private:
    std::array<T, Capacity> dense_components{};
    std::array<Entity, Capacity> dense_to_entity{};
    std::array<std::size_t, Capacity> sparse_to_dense;
    std::size_t num_components{0};
};

class ECS
{
public:
    ECS() = default;
    ~ECS() = default;

    Entity createEntity()
    {
        if (nextEntity >= MAX_ENTITIES)
        {
            return INVALID_ENTITY;
        }
        Entity id = nextEntity;
        nextEntity++;
        return id;
    }

    template <typename T>
    void addComponent(Entity entity, const T& component)
    {
        getPool<T>().add(entity, component);
    }

    template <typename T>
    T& getComponent(Entity entity)
    {
        return getPool<T>().get(entity);
    }

    template <typename T>
    bool hasComponent(Entity entity)
    {
        return getPool<T>().has(entity);
    }

    void destroyEntity(Entity entity)
    {
        if (entity == INVALID_ENTITY || entity >= nextEntity)
        {
            return;
        }

        for (const std::unique_ptr<IComponentPool>& pool : pools)
        {
            if (pool != nullptr)
            {
                pool->remove(entity);
            }
        }
    }

    template <typename T>
    std::size_t getComponentCount()
    {
        return getPool<T>().size();
    }

    template <typename T>
    Entity getEntityFromDenseIndex(std::size_t index)
    {
        return getPool<T>().getEntityAtDenseIndex(index);
    }

private:
    std::array<std::unique_ptr<IComponentPool>, MAX_COMPONENT_TYPES> pools{};
    Entity nextEntity{0};

    template <typename T>
    ComponentPool<T, MAX_ENTITIES>& getPool()
    {
        std::uint32_t id = ComponentIdCounter::getId<T>();

        if (pools[id] == nullptr)
        {
            pools[id] = std::make_unique<ComponentPool<T, MAX_ENTITIES>>();
        }

        IComponentPool* raw_base_ptr = pools[id].get();
        ComponentPool<T, MAX_ENTITIES>* derived_ptr =
            static_cast<ComponentPool<T, MAX_ENTITIES>*>(raw_base_ptr);

        return *derived_ptr;
    }
};