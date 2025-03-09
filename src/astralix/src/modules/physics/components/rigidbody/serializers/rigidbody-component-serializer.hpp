#pragma once

#include "components/rigidbody/rigidbody-component.hpp"
#include "json/json.h"

#include "components/serializers/component-serializer.hpp"

namespace astralix {
  class RigidBodyComponent;
  class RigidBodyComponentSerializer : public ComponentSerializer {

  public:
    RigidBodyComponentSerializer(RigidBodyComponent* rigidbody_component);
    RigidBodyComponentSerializer();

    void serialize() override;
    void deserialize() override;
  };
} // namespace astralix
