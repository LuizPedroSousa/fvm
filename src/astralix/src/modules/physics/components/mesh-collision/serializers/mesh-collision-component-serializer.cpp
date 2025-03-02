#include "components/mesh-collision/serializers/mesh-collision-component-serializer.hpp"
#include "components/mesh-collision/mesh-collision-component.hpp"
#include "filesystem"
#include "json/json.h"

namespace astralix {

  MeshCollisionComponentSerializer::MeshCollisionComponentSerializer(
    MeshCollisionComponent* mesh_collision_component)
    : ComponentSerializer(mesh_collision_component) {
  }

  MeshCollisionComponentSerializer::MeshCollisionComponentSerializer() {}

  Json::Value MeshCollisionComponentSerializer::serialize() {
    ComponentSerializer::serialize();
    Json::Value root = ComponentSerializer::serialize();

    return root;
  }

  void MeshCollisionComponentSerializer::save() {}

  void MeshCollisionComponentSerializer::deserialize() {}

} // namespace astralix
