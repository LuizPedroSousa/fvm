#include "components/mesh-collision/serializers/mesh-collision-component-serializer.hpp"
#include "components/mesh-collision/mesh-collision-component.hpp"
#include "filesystem"
#include "json/json.h"

namespace astralix {

MeshCollisionComponentSerializer::MeshCollisionComponentSerializer(
    MeshCollisionComponent *mesh_collision_component)
    : ComponentSerializer(mesh_collision_component) {}

MeshCollisionComponentSerializer::MeshCollisionComponentSerializer() {}

void MeshCollisionComponentSerializer::serialize() {
  ComponentSerializer::serialize();
  SerializationContext &ctx = *m_ctx.get();
}

void MeshCollisionComponentSerializer::deserialize() {}

} // namespace astralix
