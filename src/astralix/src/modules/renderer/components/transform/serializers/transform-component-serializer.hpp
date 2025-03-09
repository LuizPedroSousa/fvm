#pragma once

#include "serializer.hpp"
#include "json/json.h"

#include "components/transform/transform-component.hpp"
#include "components/serializers/component-serializer.hpp"

namespace astralix {
  class TransformComponent;
  class TransformComponentSerializer : public ComponentSerializer {

  public:
    TransformComponentSerializer(TransformComponent* transform_component);
    TransformComponentSerializer();

    void serialize() override;
    void deserialize() override;
  };
} // namespace astralix
