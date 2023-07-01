#pragma once

#include "ecs/guid.hpp"

namespace astralix {

class IComponent {
  friend class ComponentManager;

public:
  IComponent(const EntityID &owner_id, const ComponentID &component_id,
             const std::string &name, const bool &is_removable)
      : m_owner_id(owner_id), name(name), m_component_id(component_id),
        m_active(true), is_removable(is_removable) {}

  virtual ~IComponent() = default;

  IComponent() {}

  inline const ComponentID &get_component_id() const {
    return this->m_component_id;
  }

  inline const EntityID &get_owner_id() const { return this->m_owner_id; }

  virtual ComponentTypeID get_component_type_id() const = 0;

  inline bool operator==(const IComponent &other) const {
    return this->m_component_id == other.m_component_id;
  }
  inline bool operator!=(const IComponent &other) const {
    return this->m_component_id != other.m_component_id;
  }
  inline bool operator==(const IComponent *other) const {
    return this->m_component_id == other->m_component_id;
  }
  inline bool operator!=(const IComponent *other) const {
    return this->m_component_id != other->m_component_id;
  }

  inline void set_active(const bool active) { this->m_active = active; }
  bool &is_active() { return this->m_active; }

  bool is_removable;
  std::string name;

protected:
  EntityID m_owner_id;
  ComponentID m_component_id;
  bool m_active;
};

} // namespace astralix
