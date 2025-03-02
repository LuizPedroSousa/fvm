#include "rigidbody-component-serializer.hpp"
#include "components/rigidbody/serializers/rigidbody-component-serializer.hpp"
#include "filesystem"
#include "json/json.h"

namespace astralix {

RigidBodyComponentSerializer::RigidBodyComponentSerializer(
    RigidBodyComponent *rigidbody_component)
    : ComponentSerializer(rigidbody_component) {}

RigidBodyComponentSerializer::RigidBodyComponentSerializer() {}

Json::Value RigidBodyComponentSerializer::serialize() {
  ComponentSerializer::serialize();

  Json::Value root = ComponentSerializer::serialize();

  return root;
}

void RigidBodyComponentSerializer::save() {}

void RigidBodyComponentSerializer::deserialize() {}

} // namespace astralix
