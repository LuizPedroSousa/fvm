#pragma once

#include "serializer.hpp"
#include "json/json.h"

#include "components/camera/camera-component.hpp"

#include "components/serializers/component-serializer.hpp"

namespace astralix {
  class CameraComponent;
  class CameraComponentSerializer : public ComponentSerializer {

  public:
    CameraComponentSerializer(CameraComponent* camera_component);
    CameraComponentSerializer();

    void serialize() override;
    void deserialize() override;
  };
} // namespace astralix
