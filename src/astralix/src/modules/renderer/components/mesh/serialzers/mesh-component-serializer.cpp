#include "mesh-component-serializer.hpp"
#include "components/mesh/mesh-component.hpp"
#include "filesystem"
#include "json/json.h"

namespace astralix {

MeshComponentSerializer::MeshComponentSerializer(MeshComponent *mesh_component)
    : ComponentSerializer(mesh_component) {}

MeshComponentSerializer::MeshComponentSerializer() {}

void MeshComponentSerializer::serialize() {}

void MeshComponentSerializer::deserialize() {}

} // namespace astralix
