#pragma once
#include "ecs/components/component.hpp"
#include "ecs/guid.hpp"
#include "either.hpp"
#include "iostream"
#include "memory"
#include "unordered_map"

namespace astralix {

using IComponent_ptr = std::unique_ptr<IComponent>;

class ComponentManager {

public:
  ComponentManager() = default;
  ~ComponentManager() = default;

  template <typename T, typename... Args>
  T *add_component(const EntityID &entity_id, Args &&...params) {
    ComponentID id = FamilyObjectID<IComponent>::get();

    std::unique_ptr<T> component_ptr =
        std::make_unique<T>(entity_id, id, std::forward<Args>(params)...);

    const ComponentTypeID component_type_id = T::component_type_id();

    m_entity_component_table[entity_id][component_type_id] = id;

    auto created_component =
        m_component_table.emplace(id, std::move(component_ptr));

    ASSERT_THROW(!created_component.second, "Can't create a new Component !");

    return dynamic_cast<T *>(m_component_table[id].get());
  }

  template <typename T> void remove_component(const EntityID &entity_id) {
    const ComponentTypeID type_id = T::component_type_id();
    const ComponentID id =
        this->m_entity_component_table.at(entity_id).at(type_id);

    m_component_table.erase(id);
    m_entity_component_table.at(entity_id).erase(type_id);
  }

  void clean_components(const EntityID entity_id);

  template <typename T> std::vector<T *> get_components() {
    const ComponentTypeID type_id = T::component_type_id();

    std::vector<T *> components;
    for (const auto &pair : m_component_table) {
      T *component = dynamic_cast<T *>(pair.second.get());
      if (component != nullptr) {
        components.push_back(component);
      }
    }

    return components;
  }

  template <typename T> T *get_component(const EntityID entity_id) {
    const ComponentTypeID type_id = T::component_type_id();
    try {
      const ComponentID id =
          this->m_entity_component_table.at(entity_id).at(type_id);
      return dynamic_cast<T *>(m_component_table[id].get());
    } catch (std::out_of_range) {
      return nullptr;
    }
  }

private:
  std::unordered_map<ComponentID, IComponent_ptr> m_component_table;
  std::unordered_map<EntityID, std::unordered_map<ComponentTypeID, ComponentID>>
      m_entity_component_table;
};

}; // namespace astralix
