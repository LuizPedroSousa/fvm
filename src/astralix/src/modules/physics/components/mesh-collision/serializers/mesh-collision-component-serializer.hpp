#pragma once

#include "components/mesh-collision/mesh-collision-component.hpp"
#include "serializer.hpp"
#include "json/json.h"

#include "ecs/components/serializers/component-serializer.hpp"

namespace astralix {
  class MeshCollisionComponentSerializer : public ComponentSerializer {

  public:
    MeshCollisionComponentSerializer(
      MeshCollisionComponent* mesh_collision_component);
    MeshCollisionComponentSerializer();

    Json::Value serialize() override;
    void save() override;
    void deserialize() override;
  };
} // namespace astralix