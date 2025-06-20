#pragma once

#include "ecs/Entity.hpp"
#include "ecs/component/ComponentList.hpp"

#include <unordered_map>

namespace mc::ecs
{
/**
 * @brief Manages storage of ECS components.
 *
 * Stores and retrieves components of arbitrary types associated with entities.
 * Uses static storage per component type to simulate component pools.
 */
class ComponentStorage
{
public:
    /**
     * @brief Adds a component of type T to the specified entity.
     *
     * @tparam T Component type.
     * @param entity Target entity.
     * @param component Component instance to store.
     */
    template <typename T>
    void add(Entity entity, T component)
    {
        getStorage<T>()[entity] = component;
    }

    /**
     * @brief Removes a component of type T by a specified entity.
     *
     * @tparam T Component type.
     * @param entity Target entity.
     */
    template <typename T>
    void remove(Entity entity)
    {
        getStorage<T>().erase(entity);
    }

    template <typename T>
    bool has(Entity entity) const
    {
        return getStorage<T>().contains(entity);
    }

    void removeByEntity(Entity entity)
    {
        auto removeFunc = [&]<typename... T>() {
            (remove<T>(entity), ...);
        };

        [removeFunc]<typename... T>(std::type_identity<std::tuple<T...>>) {
            removeFunc.template operator()<T...>();
        }(std::type_identity<AllComponentTypes>{});
    }

    /**
     * @brief Retrieves a component of type T for the given entity.
     *
     * @tparam T Component type.
     * @param entity Target entity.
     * @return std::optional<T> Component if present, otherwise std::nullopt.
     */
    template <typename T>
    T* get(Entity entity)
    {
        auto& storage = getStorage<T>();
        auto it = storage.find(entity);
        if (it != storage.end())
        {
            return &it->second;
        }
        return nullptr;
    }

    /**
     * @brief Returns all components of type T.
     *
     * @tparam T Component type.
     * @return Reference to the unordered map of Entity to component T.
     */
    template <typename T>
    std::unordered_map<Entity, T>& getAll()
    {
        return getStorage<T>();
    }

    /**
     * @brief Clears all components of type T.
     *
     * @tparam T Component type.
     */
    template <typename T>
    void clear()
    {
        getStorage<T>().clear();
    }

private:
    /**
     * @brief Internal storage accessor for component type T.
     *
     * Uses a static unordered_map to simulate a separate storage pool for each component type.
     *
     * @tparam T Component type.
     * @return Reference to the component storage map.
     */
    template <typename T>
    std::unordered_map<Entity, T>& getStorage()
    {
        static std::unordered_map<Entity, T> storage;
        return storage;
    }
};
} // namespace mc::ecs
