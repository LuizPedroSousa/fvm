#include "rigidbody-component-serializer.hpp"
#include "components/rigidbody/serializers/rigidbody-component-serializer.hpp"
#include "filesystem"
#include "json/json.h"

namespace astralix {

  RigidBodyComponentSerializer::RigidBodyComponentSerializer(
    RigidBodyComponent* rigidbody_component)
    : ComponentSerializer(rigidbody_component) {
  }

  RigidBodyComponentSerializer::RigidBodyComponentSerializer() {}

  Json::Value RigidBodyComponentSerializer::parse() {
    ComponentSerializer::parse();

    Json::Value root = ComponentSerializer::parse();

    return root;
  }

  void RigidBodyComponentSerializer::serialize() {}

  void RigidBodyComponentSerializer::deserialize() {}

} // namespace astralix
