#include "components/transform/serializers/transform-component-serializer.hpp"
#include "components/transform/transform-component.hpp"
#include "filesystem"
#include "json/json.h"

namespace astralix {

TransformComponentSerializer::TransformComponentSerializer(
    TransformComponent *transform_component)
    : ComponentSerializer(transform_component) {}

TransformComponentSerializer::TransformComponentSerializer() {}

void TransformComponentSerializer::serialize() {
  ComponentSerializer::serialize();

  auto component = (TransformComponent *)m_component;
}

void TransformComponentSerializer::deserialize() {}

} // namespace astralix
