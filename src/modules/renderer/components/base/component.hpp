#pragma once
#include "icomponent.hpp"
#include "utils/guid.hpp"

template <typename T>
class Component : public IComponent {
  public:
  const static ComponentTypeID COMPONENT_TYPE_ID;

  ComponentTypeID get_component_type_id() const override { return COMPONENT_TYPE_ID; }
  static ComponentTypeID component_type_id() { return COMPONENT_TYPE_ID; }

  Component(COMPONENT_INIT_PARAMS) : IComponent(owner_id, component_id) {}

  Component() {}

  virtual ~Component() {}
};

template <typename T>
const ComponentTypeID Component<T>::COMPONENT_TYPE_ID = FamilyTypeID<IComponent>::get<T>();
