#pragma once

#include "serializer.hpp"
#include "json/json.h"

#include "components/material/material-component.hpp"

#include "components/serializers/component-serializer.hpp"

namespace astralix {
  class ResourceComponent;
  class ResourceComponentSerializer : public ComponentSerializer {

  public:
    ResourceComponentSerializer(ResourceComponent* resource_component);
    ResourceComponentSerializer();

    void serialize() override;
    void deserialize() override;
  };
} // namespace astralix
