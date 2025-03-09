#pragma once

#include "components/mesh-collision/mesh-collision-component.hpp"

namespace astralix {
class MeshCollisionComponentSerializer : public ComponentSerializer {

public:
  MeshCollisionComponentSerializer(
      MeshCollisionComponent *mesh_collision_component);
  MeshCollisionComponentSerializer();

  void serialize() override;
  void deserialize() override;
};
} // namespace astralix
