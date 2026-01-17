#pragma once

#include "serializer.hpp"
#include "json/json.h"

#include "components/material/material-component.hpp"

#include "components/serializers/component-serializer.hpp"

namespace astralix {
  class ModelComponent;
  class ModelComponentSerializer : public ComponentSerializer {

  public:
    ModelComponentSerializer(ModelComponent* model_component);
    ModelComponentSerializer();

    void serialize() override;
    void deserialize() override;
  };
} // namespace astralix
