#include "components/camera/serializers/camera-component-serializer.hpp"
#include "filesystem"
#include "json/json.h"

namespace astralix {

  CameraComponentSerializer::CameraComponentSerializer(CameraComponent* rigidbody_component)
    : ComponentSerializer(rigidbody_component) {
  }

  CameraComponentSerializer::CameraComponentSerializer() {}

  Json::Value CameraComponentSerializer::parse() {
    ComponentSerializer::parse();

    Json::Value root = ComponentSerializer::parse();

    return root;
  }

  void CameraComponentSerializer::serialize() {}

  void CameraComponentSerializer::deserialize() {}

} // namespace astralix
