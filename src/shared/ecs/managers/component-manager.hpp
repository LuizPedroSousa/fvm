#pragma once
#include "assert.hpp"
#include "base-manager.hpp"
#include "base.hpp"
#include "components/icomponent.hpp"
#include "guid.hpp"
#include "iostream"
#include "memory"
#include "unordered_map"
#include "vector"
#include <algorithm>

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

    ComponentID component_id = FamilyObjectID<IComponent>::get();

    Scope<T> component_ptr =
        create_scope<T>(entity_id, component_id, std::forward<Args>(params)...);

    const ComponentTypeID component_type_id = T::component_type_id();

    m_entity_component_table[entity_id][component_type_id] = component_id;

    auto created_component =
        m_component_table.emplace(component_id, std::move(component_ptr));

    ASTRA_EXCEPTION(!created_component.second,
                    "Can't create a new Component !");

    auto components_by_type = m_type_component_table.find(component_type_id);

    if (components_by_type == m_type_component_table.end()) {
      m_type_component_table[component_type_id] = {component_id};
    } else {
      components_by_type->second.push_back(component_id);
    }

    return static_cast<T *>(m_component_table[component_id].get());
  }

  template <typename T> void remove_component(const EntityID &entity_id) {

    const ComponentTypeID type_id = T::component_type_id();
    auto component_id = this->m_entity_component_table[entity_id][type_id];

    m_component_table.erase(component_id);

    auto components_by_type = m_type_component_table.find(type_id);

    if (components_by_type != m_type_component_table.end()) {
      components_by_type->second.erase(
          std::remove_if(components_by_type->second.begin(),
                         components_by_type->second.end(),
                         [&](ComponentID id) { return id == component_id; }),
          components_by_type->second.end());
    }
  }

  void clean_components(const EntityID entity_id);

  template <typename T> std::vector<T *> get_components() {
    const ComponentTypeID type_id = T::component_type_id();

    std::vector<T *> components;

    auto component_ids_by_type = m_type_component_table.find(type_id);

    if (component_ids_by_type == m_type_component_table.end())
      return components;

    for (const auto id : component_ids_by_type->second) {

      auto component = m_component_table.find(id);

      if (component == m_component_table.end())
        continue;

      components.push_back(static_cast<T *>(component->second.get()));
    }

    return components;
  }

  // template <typename T> T *get_component() {
  //   const ComponentTypeID type_id = T::component_type_id();
  //
  //   auto component_type = m_type_component_table.find(type_id);
  //
  //   if (component_type == m_type_component_table.end()) {
  //     return nullptr;
  //   }
  //
  //
  //   auto component = m_component_table.find(component_type->second);
  //
  //   return static_cast<T *>(component->second.get());
  // }

  template <typename T, typename... Args>
  T *get_or_add_component(const EntityID &entity_id, Args &&...params) {
    T *component_exists = get_component<T>(entity_id);

    if (component_exists == nullptr) {
      return add_component<T>(entity_id, std::forward<Args>(params)...);
    }

    return component_exists;
  }

  template <typename T> T *get_component(const EntityID entity_id) {
    try {

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
  std::unordered_map<ComponentTypeID, std::vector<ComponentID>>
      m_type_component_table;

  std::unordered_map<EntityID, std::unordered_map<ComponentTypeID, ComponentID>>
      m_entity_component_table;
};

}; // namespace astralix
