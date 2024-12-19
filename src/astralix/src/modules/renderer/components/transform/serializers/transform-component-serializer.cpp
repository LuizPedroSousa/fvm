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

  Json::Value TransformComponentSerializer::parse() {
    ComponentSerializer::parse();

    Json::Value root = ComponentSerializer::parse();

    return root;
  }

  void TransformComponentSerializer::serialize() {}

  void TransformComponentSerializer::deserialize() {}

} // namespace astralix
