#pragma once

#include "ecs/entities/entity.hpp"
#include "ecs/guid.hpp"
#include "functional"
#include "unordered_map"

#include "base.hpp"

namespace astralix {

class EntityManager {
public:
  explicit EntityManager(ComponentManager *component_manager);
  ~EntityManager();
  template <typename T, typename... Args> T &add_entity(Args &&...params) {
    EntityID id         = FamilyObjectID<IEntity>::get();
    Scope<T> entity_ptr = create_scope<T>(id, this->m_component_manager,
                                          std::forward<Args>(params)...);

    auto created_entity = m_entity_table.emplace(id, std::move(entity_ptr));

    ASSERT_THROW(!created_entity.second, "Error creating new Entity!");

    return dynamic_cast<T &>(*m_entity_table[id].get());
  }

  void destroy_entity(const EntityID &entity_id);

  IEntity *get_entity(const EntityID &entity_id);

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

private:
  std::unordered_map<EntityID, Scope<IEntity>> m_entity_table;
  ComponentManager *m_component_manager;
};

}; // namespace astralix
