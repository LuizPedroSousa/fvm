#pragma once
#include "ecs/guid.hpp"
#include "ecs/managers/component-manager.hpp"
#include "string"

#define ENTITY_INIT_PARAMS                                                     \
  const astralix::EntityID &id, astralix::EntityFamilyID &family_id,           \
      const std::string &name

#define ENTITY_INIT() Entity(id, family_id, name)

namespace astralix {
  class IEntity {
    friend class EntityManager;

  public:
    IEntity(ENTITY_INIT_PARAMS)
      : m_entity_id(id), m_family_id(m_family_id), name(name), m_active(true) {
    }

    IEntity() {}

    virtual ~IEntity() = default;

    void set_active(const bool active) {
      if (m_active == active)
        return;

      if (!active)
        on_disable();
      else
        on_enable();

      m_active = active;
    }

    inline const EntityID& get_entity_id() const { return m_entity_id; }
    virtual EntityTypeID get_entity_type_id() const = 0;

    inline bool is_active() const { return this->m_active; }

    template <typename T> T* get_component() const {
      return ComponentManager::get()->get_component<T>(m_entity_id);
    }
    std::vector<IComponent*> get_components() const {
      auto manager = ComponentManager::get();
      auto entity_components_table =
        manager->m_entity_component_table.at(m_entity_id);

      std::vector<IComponent*> components;

      for (auto entity_component = entity_components_table.begin(),
        end = entity_components_table.end();
        entity_component != end; entity_component++) {


        auto component = manager->m_component_table.at(entity_component->second).get();
        components.push_back(
          component
        );
      }

      return components;
    }

    template <typename T, typename... Args>
    T* get_or_add_component(Args &&...params) const {
      return ComponentManager::get()->get_or_add_component<T>(
        m_entity_id, std::forward(params)...);
    }

    template <typename T, typename... Args> T* add_component(Args &&...params) {
      return ComponentManager::get()->add_component<T>(
        m_entity_id, std::forward<Args>(params)...);
    }

    template <typename T> bool has_component() {
      auto component = get_component<T>();

      return component != nullptr;
    };

    template <typename T> void remove_component() {
      return ComponentManager::get()->remove_component<T>(m_entity_id);
    }

    virtual void on_enable() = 0;
    virtual void on_disable() = 0;

    // helpers for comparassion between entities
    inline bool operator==(const IEntity& other) const {
      return m_entity_id == other.m_entity_id;
    }
    inline bool operator!=(const IEntity& other) const {
      return m_entity_id != other.m_entity_id;
    }
    inline bool operator==(const IEntity* other) const {
      return m_entity_id == other->m_entity_id;
    }
    inline bool operator!=(const IEntity* other) const {
      return m_entity_id != other->m_entity_id;
    }

    std::string name;

  protected:
    EntityID m_entity_id;
    EntityFamilyID m_family_id;
    bool m_active;
  };

} // namespace astralix
