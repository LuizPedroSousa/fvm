#pragma once

#include "utils/guid.hpp"

class EntityManager;

#define COMPONENT_INIT_PARAMS const EntityID &owner_id, const ComponentID &component_id
#define COMPONENT_INIT(Type) Component<Type>(owner_id, component_id)

class IComponent {
  friend class ComponentManager;

  public:
  IComponent(COMPONENT_INIT_PARAMS) : m_owner_id(owner_id),
                                      m_component_id(component_id), m_active(true) {}

  virtual ~IComponent() = default;

  IComponent() {}

  inline const ComponentID &get_component_id() const { return this->m_component_id; }
  inline const EntityID &get_owner_id() const { return this->m_owner_id; }

  virtual ComponentTypeID get_component_type_id() const = 0;

  inline bool operator==(const IComponent &other) const { return this->m_component_id == other.m_component_id; }
  inline bool operator!=(const IComponent &other) const { return this->m_component_id != other.m_component_id; }
  inline bool operator==(const IComponent *other) const { return this->m_component_id == other->m_component_id; }
  inline bool operator!=(const IComponent *other) const { return this->m_component_id != other->m_component_id; }

  inline void set_active(const bool active) { this->m_active = active; }
  inline bool is_active() const { return this->m_active; }

  protected:
  EntityID m_owner_id;
  ComponentID m_component_id;
  bool m_active;
};
