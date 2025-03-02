#pragma once

#include "components/rigidbody/rigidbody-component.hpp"
#include "serializer.hpp"
#include "json/json.h"

#include "ecs/components/serializers/component-serializer.hpp"

namespace astralix {
  class RigidBodyComponent;
  class RigidBodyComponentSerializer : public ComponentSerializer {

  public:
    RigidBodyComponentSerializer(RigidBodyComponent* rigidbody_component);
    RigidBodyComponentSerializer();

    Json::Value serialize() override;
    void save() override;
    void deserialize() override;
  };
} // namespace astralix