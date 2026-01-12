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

  SerializationContext &ctx = *m_ctx.get();

  ctx["position"]["x"] = component->position.x;
  ctx["position"]["y"] = component->position.y;
  ctx["position"]["z"] = component->position.z;

  ctx["scale"]["x"] = component->scale.x;
  ctx["scale"]["y"] = component->scale.y;
  ctx["scale"]["z"] = component->scale.z;

  ctx["rotation"]["x"] = component->rotation.x;
  ctx["rotation"]["y"] = component->rotation.y;
  ctx["rotation"]["z"] = component->rotation.z;
}

void TransformComponentSerializer::deserialize() {}

} // namespace astralix
