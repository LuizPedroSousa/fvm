#pragma once
#include "ecs/guid.hpp"
#include "ecs/managers/component-manager.hpp"

#define ENTITY_INIT_PARAMS                                                     \
  const astralix::EntityID &id, astralix::ComponentManager *component_manager
#define ENTITY_INIT() Entity(id, component_manager)

namespace astralix {
class IEntity {
  friend class EntityManager;

private:
  ComponentManager *m_component_manager;

protected:
  EntityID m_entity_id;
  bool m_active;

public:
  IEntity(ENTITY_INIT_PARAMS)
      : m_component_manager(component_manager), m_entity_id(id),
        m_active(true) {}

  IEntity() {}

  virtual ~IEntity() = default;

  template <typename T> T *get_component() const {
    return this->m_component_manager->get_component<T>(m_entity_id);
  }

  template <typename T, typename... Args> T *add_component(Args &&...params) {
    return m_component_manager->add_component<T>(m_entity_id,
                                                 std::forward<Args>(params)...);
  }

  template <typename T> void remove_component() {
    return m_component_manager->remove_component<T>(m_entity_id);
  }

  void set_active(const bool active) {
    if (m_active == active)
      return;

    if (!active)
      on_disable();
    else
      on_enable();

    m_active = active;
  }

  inline const EntityID &get_entity_id() const { return m_entity_id; }
  virtual EntityTypeID get_entity_type_id() const = 0;

  inline bool is_active() const { return this->m_active; }

  virtual void on_enable() = 0;
  virtual void on_disable() = 0;

  // helpers for comparassion between entities
  inline bool operator==(const IEntity &other) const {
    return m_entity_id == other.m_entity_id;
  }
  inline bool operator!=(const IEntity &other) const {
    return m_entity_id != other.m_entity_id;
  }
  inline bool operator==(const IEntity *other) const {
    return m_entity_id == other->m_entity_id;
  }
  inline bool operator!=(const IEntity *other) const {
    return m_entity_id != other->m_entity_id;
  }
};

} // namespace astralix
