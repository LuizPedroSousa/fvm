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

namespace astralix {

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

    ASTRA_ASSERT_THROW(!created_component.second,
                       "Can't create a new Component !");

    return dynamic_cast<T *>(m_component_table[component_id].get());
  }

  template <typename T> void remove_component(const EntityID &entity_id) {

    const ComponentTypeID type_id = T::component_type_id();
    const ComponentID component_id =
        this->m_entity_component_table.at(entity_id).at(type_id);

    m_component_table.erase(component_id);
    m_entity_component_table.at(entity_id).erase(type_id);
  }

  void clean_components(const EntityID entity_id);

  template <typename T> std::vector<T *> get_components() {
    const ComponentTypeID type_id = T::component_type_id();

    std::vector<T *> components;
    for (const auto &pair : m_component_table) {
      Scope<IComponent> &component = pair.second;
      if (component != nullptr) {
        components.push_back(dynamic_cast<T *>(component.get()));
      }
    }

    return components;
  }

  template <typename T> T *get_component() {
    const ComponentTypeID type_id = T::component_type_id();

    for (const auto &pair : m_component_table) {
      const Scope<IComponent> &component = pair.second;

      if (component->get_component_type_id() == type_id) {
        return dynamic_cast<T *>(component.get());
      }
    }

    return nullptr;
  }

  template <typename T, typename... Args>
  T *get_or_add_component(const EntityID &entity_id, Args &&...params) {
    T *component_exists = get_component<T>();

    if (component_exists == nullptr) {
      return add_component<T>(entity_id, std::forward<Args>(params)...);
    }

    return component_exists;
  }

  template <typename T> T *get_component(const EntityID entity_id) {
    const ComponentTypeID type_id = T::component_type_id();
    try {
      const ComponentID component_id =
          this->m_entity_component_table.at(entity_id).at(type_id);
      return dynamic_cast<T *>(m_component_table[component_id].get());
    } catch (std::out_of_range) {
      return nullptr;
    }
  }

  ComponentManager() = default;

private:
  std::unordered_map<ComponentID, Scope<IComponent>> m_component_table;
  std::unordered_map<EntityID, std::unordered_map<ComponentTypeID, ComponentID>>
      m_entity_component_table;
};

}; // namespace astralix
