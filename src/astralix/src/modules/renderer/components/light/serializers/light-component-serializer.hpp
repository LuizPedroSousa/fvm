#pragma once

#include "serializer.hpp"
#include "json/json.h"

#include "components/camera/camera-component.hpp"

#include "components/serializers/component-serializer.hpp"

namespace astralix {
  class LightComponent;
  class LightComponentSerializer : public ComponentSerializer {

  public:
    LightComponentSerializer(LightComponent* light_component);
    LightComponentSerializer();

    void serialize() override;
    void deserialize() override;
  };
} // namespace astralix
