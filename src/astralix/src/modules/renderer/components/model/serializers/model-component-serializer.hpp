#pragma once

#include "serializer.hpp"
#include "json/json.h"

#include "components/material/material-component.hpp"

#include "ecs/components/serializers/component-serializer.hpp"

namespace astralix {
  class ModelComponent;
  class ModelComponentSerializer : public ComponentSerializer {

  public:
    ModelComponentSerializer(ModelComponent* model_component);
    ModelComponentSerializer();

    Json::Value serialize() override;
    void save() override;
    void deserialize() override;
  };
} // namespace astralix