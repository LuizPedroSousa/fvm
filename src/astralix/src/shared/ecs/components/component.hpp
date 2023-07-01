#pragma once
#include "ecs/managers/entity-manager.hpp"
#include "engine.hpp"

namespace astralix {

#define COMPONENT_INIT_PARAMS                                                  \
  const EntityID &owner_id, const ComponentID &component_id

#define COMPONENT_INIT(Type, Name, Is_Removable)                               \
  Component<Type>(owner_id, component_id, Name, Is_Removable)

class IEntity;

template <typename T> class Component : public IComponent {
public:
  const static ComponentTypeID s_component_type_id;

  ComponentTypeID get_component_type_id() const override {
    return s_component_type_id;
  }
  static ComponentTypeID component_type_id() { return s_component_type_id; }

  Component(COMPONENT_INIT_PARAMS, const std::string &name,
            const bool &is_removable)
      : IComponent(owner_id, component_id, name, is_removable) {}

  IEntity *get_owner() {
    return EntityManager::get()->get_entity(get_owner_id());
  }

  template <typename E> E *get_owner() {
    return dynamic_cast<E *>((IEntity *)get_owner());
  }

  Component() {}

  virtual ~Component() {}
};

template <typename T>
const ComponentTypeID Component<T>::s_component_type_id =
    FamilyTypeID<IComponent>::get<T>();

} // namespace astralix
