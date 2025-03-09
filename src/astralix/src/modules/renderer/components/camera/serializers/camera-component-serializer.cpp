#include "components/camera/serializers/camera-component-serializer.hpp"
#include "filesystem"
#include "json/json.h"

namespace astralix {

CameraComponentSerializer::CameraComponentSerializer(
    CameraComponent *rigidbody_component)
    : ComponentSerializer(rigidbody_component) {}

CameraComponentSerializer::CameraComponentSerializer() {}

void CameraComponentSerializer::serialize() {
  ComponentSerializer::serialize();
}

void CameraComponentSerializer::deserialize() {}

} // namespace astralix
