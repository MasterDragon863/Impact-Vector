#pragma once

#include <array>
#include <cstdint>
#include <limits>
#include <memory>

using Entity = std::uint32_t;
constexpr Entity MAX_ENTITIES{10};
constexpr Entity INVALID_ENTITY{std::numeric_limits<Entity>::max()};
constexpr std::size_t MAX_COMPONENT_TYPES{32};

/**
 * @class ComponentIdCounter
 * @brief static class to generate unique ids for every component
 *
 */
struct ComponentIdCounter {
  inline static std::uint32_t nextTypeId{0};

  /**
   * @brief Generates a unique id for a given type
   *
   * @tparam T the type to generate an id for
   * @return the id for the type T
   */
  template <typename T>
  static std::uint32_t getId() {
    static const std::uint32_t id = nextTypeId++;
    return id;
  }
};

/**
 * @class IComponentPool
 * @brief Parent virtual Interface to make the ecs cleaner
 *
 * Created to be able to have a single array for every component type.
 *
 */
class IComponentPool {
 public:
  virtual ~IComponentPool() = default;
  /**
   * @brief erases an entity id and removes the data from the pool
   *
   * @param entity - The entity Id to remove
   */
  virtual void remove(Entity entity) = 0;

  /**
   * @brief Checks if an entity has a component in this pool
   *
   * @param entity - the Entity Id to check
   * @return - true if the object exists, false otherwise
   */
  virtual bool has(Entity entity) const = 0;
};

/**
 * @class ComponentPool
 * @brief Class that implements a sparse set to cleanl organize data to be CPU
 * cache friendly This data structure maps a widely scattered Entity ID space
 * into a densely packed block of arrays, optimizing CPU cache line hits during
 * sequential entity processing iterations.
 * @tparam T - The Componenet type to create a sparse set for
 * @param Capacity - Maximum number of components to be create, recommended to
 * be max entity numberj
 */
template <typename T, std::size_t Capacity>
class ComponentPool : public IComponentPool {
 public:
  ComponentPool() {
    sparse_to_dense.fill(std::numeric_limits<std::size_t>::max());
  }

  ~ComponentPool() override = default;

  /**
   * @brief Adds a Entity and Component to the sparse set
   *
   * @param entity - Entity to add
   * @param component - Component to add
   * @return true, if successfull, else false
   */
  bool add(Entity entity, const T& component) {
    if (entity >= Capacity || has(entity) || num_components >= Capacity) {
      return false;
    }

    sparse_to_dense[entity] = num_components;
    dense_to_entity[num_components] = entity;
    dense_components[num_components] = component;

    num_components++;
    return true;
  }

  /**
   * @brief Gets a component
   *
   * @param entity - entity to get component for
   * @return the component
   *
   * @warning - not safe, check with has() beforehand
   */
  T& get(Entity entity) {
    std::size_t dense_index = sparse_to_dense[entity];
    return dense_components[dense_index];
  }
  /**
   * @brief Gets a component
   *
   * @param entity - entity to get component for
   * @return const of the component
   *
   * @warning - not safe, check with has() beforehand
   */
  const T& get(Entity entity) const {
    std::size_t dense_index = sparse_to_dense[entity];
    return dense_components[dense_index];
  }

  /**
   * @brief Checks if an entity has a component
   *
   * @param entity - entity to check for
   * @return true if entity has component, else false
   */
  bool has(Entity entity) const override {
    if (entity >= Capacity) return false;
    return sparse_to_dense[entity] != std::numeric_limits<std::size_t>::max();
  }

  /**
   * @brief Deletes an Entity and its componenet from the set
   *
   * Removes the element by swapping it with the final element in array in the
   * dense set. This maintains a O(1) time.
   *
   * @param entity - entity to delete
   */
  void remove(Entity entity) override {
    if (!has(entity)) return;

    std::size_t index_to_remove = sparse_to_dense[entity];
    std::size_t last_index = num_components - 1;

    dense_components[index_to_remove] = dense_components[last_index];

    Entity last_entity = dense_to_entity[last_index];
    dense_to_entity[index_to_remove] = last_entity;

    sparse_to_dense[last_entity] = index_to_remove;
    sparse_to_dense[entity] = std::numeric_limits<std::size_t>::max();

    num_components--;
  }

  /**
   * @brief Gets the total number of acitve components in this sparse set
   */
  std::size_t size() const { return num_components; }

  /**
   * @brief Gets the Entity Based onthe index of the component in the dense array
   *
   * @param index - The index of the dense component
   * @return the parent Entity of the dense component
   */
  Entity getEntityAtDenseIndex(std::size_t index) const {
    return dense_to_entity[index];
  }

 private:
  std::array<T, Capacity> dense_components{};
  std::array<Entity, Capacity> dense_to_entity{};
  std::array<std::size_t, Capacity> sparse_to_dense;
  std::size_t num_components{0};
};

/**
 * @class ECS
 * @brief The 
 *
 */
class ECS {
 public:
  ECS() = default;
  ~ECS() = default;

  /**
   * @brief Creates an Entity id and updates the next availabe id 
   *
   * @return a valid entity if available, else INVALID_ENTITY
   */
  Entity createEntity();
  /**
   * @brief Removes an Entity from the component pools
   *
   * @param entity - Entity to remove
   */
  void destroyEntity(Entity entity);

  /**
   * @brief Adds a component to the appropriate Component Pool
   *
   * @tparam T Type of the Component you want ot add
   * @param entity - The enity you want to add the component to 
   * @param component - The component you want to add
   */
  template <typename T>
  void addComponent(Entity entity, const T& component) {
    getPool<T>().add(entity, component);
  }

  /**
   * @brief Retrieve a component 
   *
   * @tparam T - The type of the Component you want to retrieve
   * @param entity - The enity Id of the component you want to retrieve
   * @return - the component retrieved
   */
  template <typename T>
  T& getComponent(Entity entity) {
    return getPool<T>().get(entity);
  }
  /**
   * @brief Retrieve a component 
   *
   * @tparam T - The type of the Component you want to retrieve
   * @param entity - The enity Id of the component you want to retrieve
   * @return - the component retrieved
   */
  template <typename T>
  const T& getComponent(Entity entity) const {
    return getPool<T>().get(entity);
  }

  /**
   * @brief Checks if a entity has a specific component
   *
   * @tparam T - The component type you want to check for
   * @param entity - The entity you want to check their component for 
   * @return - true, if it has the component, false otherwise
   */
  template <typename T>
  bool hasComponent(Entity entity) const {
    return getPool<T>().has(entity);
  }

  /**
   * @brief Returns the the number of component in a specific component pool
   *
   * @tparam T - The Component pool type you want to check for
   * @return - the size of the acitve components in the T component pool
   */
  template <typename T>
  std::size_t getComponentCount() const {
    return getPool<T>().size();
  }

  /**
   * @brief Gets an Entity based on the dense index of the entity
   *
   * @tparam T - The type of the component you are retrieving the entity from
   * @param index - The dense index of the component
   * @return - The Entity ID
   */
  template <typename T>
  Entity getEntityFromDenseIndex(std::size_t index) const {
    return getPool<T>().getEntityAtDenseIndex(index);
  }

 private:
  mutable std::array<std::unique_ptr<IComponentPool>, MAX_COMPONENT_TYPES>
      pools{};
  Entity nextEntity{0};

  /**
   * @brief Creates or retrieve a component pool 
   *
   * @tparam T The type of the component pool wanted to be retrieved
   * @return - The Component Pool
   */
  template <typename T>
  ComponentPool<T, MAX_ENTITIES>& getPool() const {
    std::uint32_t id = ComponentIdCounter::getId<T>();

    if (pools[id] == nullptr) {
      pools[id] = std::make_unique<ComponentPool<T, MAX_ENTITIES>>();
    }

    IComponentPool* raw_base_ptr = pools[id].get();
    ComponentPool<T, MAX_ENTITIES>* derived_ptr =
        static_cast<ComponentPool<T, MAX_ENTITIES>*>(raw_base_ptr);

    return *derived_ptr;
  }
};
