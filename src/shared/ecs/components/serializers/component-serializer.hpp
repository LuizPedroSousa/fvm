#pragma once
#include "base.hpp"
#include "serializer.hpp"

#include "components/icomponent.hpp"
namespace astralix {
  class IComponent;
  class ComponentSerializer : public Serializer {
  public:
    void serialize() override;

    ComponentSerializer(IComponent* component) : m_component(component) {}
    ComponentSerializer() = default;

  protected:
    IComponent* m_component = nullptr;
  };
} // namespace astralix
