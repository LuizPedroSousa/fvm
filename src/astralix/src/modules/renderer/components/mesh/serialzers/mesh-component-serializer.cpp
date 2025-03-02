#include "mesh-component-serializer.hpp"
#include "filesystem"
#include "json/json.h"
#include "components/mesh/mesh-component.hpp"

namespace astralix {

  MeshComponentSerializer::MeshComponentSerializer(
    MeshComponent* mesh_component) : ComponentSerializer(mesh_component)
  {
  }

  MeshComponentSerializer::MeshComponentSerializer() {}

  Json::Value MeshComponentSerializer::serialize() {
    Json::Value root = ComponentSerializer::serialize();

    return root;
  }

  void MeshComponentSerializer::save() {}

  void MeshComponentSerializer::deserialize() {}

} // namespace astralix
