#pragma once
#include "json/json.h"

#include "ecs/components/serializers/component-serializer.hpp"
#include "serializer.hpp"

namespace astralix {
  class MeshComponent;
  class MeshComponentSerializer : public ComponentSerializer {

  public:
    MeshComponentSerializer(MeshComponent* mesh_component);

    MeshComponentSerializer();

    Json::Value parse() override;
    void serialize() override;
    void deserialize() override;
  };
} // namespace astralix