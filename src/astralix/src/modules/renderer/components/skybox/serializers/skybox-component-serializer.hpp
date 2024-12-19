#pragma once

#include "serializer.hpp"
#include "json/json.h"

#include "components/skybox/skybox-component.hpp"

#include "ecs/components/serializers/component-serializer.hpp"

namespace astralix {
  class SkyboxComponent;
  class SkyboxComponentSerializer : public ComponentSerializer {

  public:
    SkyboxComponentSerializer(SkyboxComponent* skybox_component);
    SkyboxComponentSerializer();

    Json::Value parse() override;
    void serialize() override;
    void deserialize() override;
  };
} // namespace astralix