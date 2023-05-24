#pragma once

#include "entities/base/entity.hpp"
#include "unordered_map"
#include "utils/guid.hpp"

using IEntity_ptr = std::shared_ptr<IEntity>;

class IEntityContainer {
  public:
  virtual ~IEntityContainer() {}

  virtual void destroy_entity(const EntityID &entity_id) = 0;
};

template <typename T>
class EntityContainer : public IEntityContainer {
  using TypedEntityContainer = std::unordered_map<EntityID, std::shared_ptr<Entity<T>>>;

  private:
  TypedEntityContainer m_typed_container;

  public:
  EntityContainer() {}
  TypedEntityContainer &get_typed_entity_container() { return m_typed_container; }
  void destroy_entity(const EntityID &entity_id) override {
    m_typed_container.erase(entity_id);
  }
};

class EntityManager {

  public:
  explicit EntityManager(ComponentManager *component_manager);
  ~EntityManager();

  template <typename T, typename... Args>
  T &add_entity(Args &&...params) {
    EntityID id = FamilyObjectID<IEntity>::get();
    auto entity_ptr = std::make_shared<T>(id, this->m_component_manager, std::forward<Args>(params)...);
    auto created_entity = m_entity_table.try_emplace(id, entity_ptr);

    ASSERT_THROW(!created_entity.second, "Error creating new Entity !");

    auto container_ptr = get_entity_container<T>();

    ASSERT_COMPARE_THROW(container_ptr);

    auto container = container_ptr.right()->get_typed_entity_container().try_emplace(id, entity_ptr);

    ASSERT_THROW(!container.second, "Error inserting Entity in typed containers !");

    return *entity_ptr.get();
  }

  void destroy_entity(const EntityID &entity_id);

  IEntity *get_entity(const EntityID &entity_id);
  std::unordered_map<EntityID, IEntity_ptr> get_entities();

  private:
  template <typename T>
  Either<BaseException, EntityContainer<T> *> get_entity_container() {
    EntityTypeID type_id = T::entity_type_id();

    auto entity_typed = this->m_entity_typed_table.find(type_id);
    if (entity_typed != m_entity_typed_table.end())
      return reinterpret_cast<EntityContainer<T> *>(entity_typed->second.get());

    auto entity_container = this->m_entity_typed_table.try_emplace(type_id, std::make_unique<EntityContainer<T>>());

    ASSERT(!entity_container.second, "Failed to create EntityContainer<T>!");

    return reinterpret_cast<EntityContainer<T> *>(entity_container.first->second.get());
  }

  using IEntityContainer_ptr = std::shared_ptr<IEntityContainer>;

  std::unordered_map<EntityTypeID, IEntityContainer_ptr> m_entity_typed_table;
  std::unordered_map<EntityID, IEntity_ptr> m_entity_table;
  ComponentManager *m_component_manager;
};