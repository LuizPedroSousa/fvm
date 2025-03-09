#pragma once
#include "json/json.h"

#include "components/serializers/component-serializer.hpp"
#include "serializer.hpp"

namespace astralix {
  class MeshComponent;
  class MeshComponentSerializer : public ComponentSerializer {

  public:
    MeshComponentSerializer(MeshComponent* mesh_component);

    MeshComponentSerializer();

    void serialize() override;
    void deserialize() override;
  };
} // namespace astralix
