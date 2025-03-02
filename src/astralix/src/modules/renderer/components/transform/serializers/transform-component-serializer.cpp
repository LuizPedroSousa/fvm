#include "components/transform/serializers/transform-component-serializer.hpp"
#include "components/transform/transform-component.hpp"
#include "filesystem"
#include "json/json.h"

namespace astralix {

  TransformComponentSerializer::TransformComponentSerializer(
    TransformComponent* transform_component)
    : ComponentSerializer(transform_component) {
  }

  TransformComponentSerializer::TransformComponentSerializer() {}

  Json::Value TransformComponentSerializer::serialize() {
    ComponentSerializer::serialize();

    Json::Value root = ComponentSerializer::serialize();

    return root;
  }

  void TransformComponentSerializer::save() {}

  void TransformComponentSerializer::deserialize() {}

} // namespace astralix
