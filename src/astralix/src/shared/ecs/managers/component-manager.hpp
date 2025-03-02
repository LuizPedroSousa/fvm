#pragma once
#include "assert.hpp"
#include "base-manager.hpp"
#include "base.hpp"
#include "ecs/components/icomponent.hpp"
#include "ecs/guid.hpp"
#include "iostream"
#include "memory"
#include "unordered_map"
#include "vector"

#include <tracy/Tracy.hpp>

namespace astralix {

class IEntity;

struct EntityComponent {
  ComponentID id;
  ComponentTypeID type;
};

class ComponentManager : public BaseManager<ComponentManager> {

public:
  ~ComponentManager() = default;

  template <typename T, typename... Args>
  T *add_component(const EntityID &entity_id, Args &&...params) {
    ZoneScopedN("ComponentManager add_component");

    ComponentID component_id = FamilyObjectID<IComponent>::get();

    Scope<T> component_ptr =
        create_scope<T>(entity_id, component_id, std::forward<Args>(params)...);

    const ComponentTypeID component_type_id = T::component_type_id();

    m_entity_component_table[entity_id][component_type_id] = component_id;

    auto created_component =
        m_component_table.emplace(component_id, std::move(component_ptr));

    ASTRA_EXCEPTION(!created_component.second,
                    "Can't create a new Component !");

    m_type_component_table[component_type_id] = component_id;

    return static_cast<T *>(m_component_table[component_id].get());
  }

  template <typename T> void remove_component(const EntityID &entity_id) {
    ZoneScopedN("ComponentManager remove_component");

    const ComponentTypeID type_id = T::component_type_id();
    auto component_id = this->m_entity_component_table[entity_id][type_id];

    m_component_table.erase(component_id);
    m_type_component_table[type_id];
  }

  void clean_components(const EntityID entity_id);

  template <typename T> std::vector<T *> get_components() {

    ZoneScopedN("ComponentManager get_components");

    const ComponentTypeID type_id = T::component_type_id();

    std::vector<T *> components;
    for (const auto &pair : m_component_table) {
      Scope<IComponent> &component = pair.second;
      if (component != nullptr) {
        components.push_back(static_cast<T *>(component.get()));
      }
    }

    return components;
  }

  template <typename T> T *get_component() {
    const ComponentTypeID type_id = T::component_type_id();

    auto component_type = m_type_component_table.find(type_id);

    if (component_type == m_type_component_table.end()) {
      return nullptr;
    }

    auto component = m_component_table.find(component_type->second);

    return static_cast<T *>(component->second.get());
  }

  template <typename T, typename... Args>
  T *get_or_add_component(const EntityID &entity_id, Args &&...params) {
    ZoneScopedN("ComponentManager get_or_add_component");
    T *component_exists = get_component<T>(entity_id);

    if (component_exists == nullptr) {
      return add_component<T>(entity_id, std::forward<Args>(params)...);
    }

    return component_exists;
  }

  template <typename T> T *get_component(const EntityID entity_id) {
    try {
      ZoneScopedN("ComponentManager get_component");

      const ComponentTypeID type_id = T::component_type_id();

      auto component_id = this->m_entity_component_table[entity_id][type_id];

      return static_cast<T *>(m_component_table[component_id].get());
    } catch (std::out_of_range) {
      return nullptr;
    }
  }

  ComponentManager() = default;

  friend IEntity;

private:
  std::unordered_map<ComponentID, Scope<IComponent>> m_component_table;
  std::unordered_map<ComponentTypeID, ComponentID> m_type_component_table;

  std::unordered_map<EntityID, std::unordered_map<ComponentTypeID, ComponentID>>
      m_entity_component_table;
};

}; // namespace astralix
