#pragma once

#include "ientity.hpp"

namespace astralix {

template <typename T> class Entity : public IEntity {
public:
  static const EntityTypeID ENTITY_TYPE_ID;

  EntityTypeID get_entity_type_id() const override { return ENTITY_TYPE_ID; }
  static EntityTypeID entity_type_id() { return ENTITY_TYPE_ID; }

  virtual void on_enable() = 0;
  virtual void on_disable() = 0;

  Entity(ENTITY_INIT_PARAMS) : IEntity(id, component_manager) {}
  Entity() {}

  ~Entity() {}
};

template <typename T>
const EntityTypeID Entity<T>::ENTITY_TYPE_ID = FamilyTypeID<IEntity>::get<T>();

} // namespace astralix
