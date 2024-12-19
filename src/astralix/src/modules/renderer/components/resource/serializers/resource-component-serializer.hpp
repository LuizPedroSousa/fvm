#pragma once

#include "serializer.hpp"
#include "json/json.h"

#include "components/material/material-component.hpp"

#include "ecs/components/serializers/component-serializer.hpp"

namespace astralix {
  class ResourceComponent;
  class ResourceComponentSerializer : public ComponentSerializer {

  public:
    ResourceComponentSerializer(ResourceComponent* resource_component);
    ResourceComponentSerializer();

    Json::Value parse() override;
    void serialize() override;
    void deserialize() override;
  };
} // namespace astralix