#include "components/camera/serializers/camera-component-serializer.hpp"
#include "filesystem"
#include "json/json.h"

namespace astralix {

  CameraComponentSerializer::CameraComponentSerializer(CameraComponent* rigidbody_component)
    : ComponentSerializer(rigidbody_component) {
  }

  CameraComponentSerializer::CameraComponentSerializer() {}

  Json::Value CameraComponentSerializer::serialize() {
    ComponentSerializer::serialize();

    Json::Value root = ComponentSerializer::serialize();

    return root;
  }

  void CameraComponentSerializer::save() {}

  void CameraComponentSerializer::deserialize() {}

} // namespace astralix
