#pragma once

#include "ecs/entities/events/entity-events.hpp"
#include "ecs/entities/ientity.hpp"
#include "ecs/guid.hpp"
#include "ecs/managers/component-manager.hpp"
#include "events/event-dispatcher.hpp"
#include "functional"
#include "unordered_map"

#include "base-manager.hpp"
#include "base.hpp"
#include "entity-manager.hpp"

namespace astralix {

class EntityManager : public BaseManager<EntityManager> {

public:
  template <typename T, typename... Args>
  T *add_entity(const std::string &&name = "GameObject", Args &&...params) {
    EntityID id = EntityID();
    EntityFamilyID family_id = FamilyObjectID<IEntity>::get();

    int count = 1;
    std::string unique_name = name;

    bool name_exists = true;
    while (name_exists) {
      name_exists = false;
      for (const auto &pair : m_entity_table) {
        const Scope<IEntity> &entity = pair.second;
        if (entity->name == unique_name) {
          name_exists = true;
          unique_name = name + " (" + std::to_string(count) + ")";
          count++;
          break;
        }
      }
    }

    Scope<T> entity_ptr = create_scope<T>(id, family_id, unique_name,
                                          std::forward<Args>(params)...);

    auto emplaced_entity = m_entity_table.emplace(id, std::move(entity_ptr));

    ASTRA_ASSERT_THROW(!emplaced_entity.second, "Error creating new Entity!");

    auto dispatcher = EventDispatcher::get();

    auto created_entity = m_entity_table[id].get();

    auto event = EntityCreatedEvent(created_entity);
    dispatcher->dispatch(&event);

    return static_cast<T *>(created_entity);
  }

  void destroy_entity(const EntityID &entity_id);

  IEntity *get_entity(const EntityID &entity_id);

  IEntity *get_entity_by_name(const std::string &name);

  template <typename T> bool has_entity_with_component() {
    auto entity = get_entity_with_component<T>();

    return entity != nullptr;
  }

  template <typename T> IEntity *get_entity_with_component() {
    for (const auto &pair : m_entity_table) {
      const Scope<IEntity> &entity = pair.second;
      if (entity->has_component<T>()) {
        return entity.get();
      }
    }

    return nullptr;
  }

  template <typename T> T *get_entity() {
    EntityTypeID type_id = T::entity_type_id();

    for (const auto &pair : m_entity_table) {
      const Scope<IEntity> &entity = pair.second;
      if (entity->get_entity_type_id() == type_id) {
        return dynamic_cast<T *>(entity.get());
      }
    }

    return nullptr;
  }

  template <typename T> T *get_entity(const EntityID &entity_id) {
    return dynamic_cast<T *>(get_entity(entity_id));
  }

  void for_each(std::function<void(IEntity *)> fn) {
    for (const auto &pair : m_entity_table) {
      const Scope<IEntity> &entity = pair.second;

      fn(entity.get());
    }
  }

  template <typename T> void for_each(std::function<void(T *)> fn) {
    std::vector<T *> entities = get_entities<T>();

    for (auto entity = entities.begin(); entity != entities.end(); entity++) {
      fn(*entity);
    }
  }

  template <typename T> std::vector<T *> get_entities() {
    std::vector<T *> result;
    EntityTypeID type_id = T::entity_type_id();

    for (const auto &pair : m_entity_table) {
      const Scope<IEntity> &entity = pair.second;
      if (entity->get_entity_type_id() == type_id) {
        result.push_back(dynamic_cast<T *>(entity.get()));
      }
    }

    return result;
  }

  explicit EntityManager();
  ~EntityManager();

private:
  std::unordered_map<EntityID, Scope<IEntity>> m_entity_table;
};

}; // namespace astralix
