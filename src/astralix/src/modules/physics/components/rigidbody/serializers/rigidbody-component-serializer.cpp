#include "components/rigidbody/serializers/rigidbody-component-serializer.hpp"
#include "filesystem"
#include "json/json.h"

namespace astralix {

RigidBodyComponentSerializer::RigidBodyComponentSerializer(
    RigidBodyComponent *rigidbody_component)
    : ComponentSerializer(rigidbody_component) {}

RigidBodyComponentSerializer::RigidBodyComponentSerializer() {}

void RigidBodyComponentSerializer::serialize() {
  ComponentSerializer::serialize();
}

void RigidBodyComponentSerializer::deserialize() {}

} // namespace astralix
