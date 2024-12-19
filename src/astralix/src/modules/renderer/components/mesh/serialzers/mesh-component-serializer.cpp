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

  Json::Value MeshComponentSerializer::parse() {
    Json::Value root = ComponentSerializer::parse();

    return root;
  }

  void MeshComponentSerializer::serialize() {}

  void MeshComponentSerializer::deserialize() {}

} // namespace astralix
