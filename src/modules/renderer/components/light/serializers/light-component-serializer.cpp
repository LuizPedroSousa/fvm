#include "components/light/serializers/light-component-serializer.hpp"
#include "components/light/light-component.hpp"
#include "filesystem"
#include "json/json.h"

namespace astralix {

LightComponentSerializer::LightComponentSerializer(
    LightComponent *light_component)
    : ComponentSerializer(light_component) {}

LightComponentSerializer::LightComponentSerializer() {}

void LightComponentSerializer::serialize() {
  ComponentSerializer::serialize();

  SerializationContext &ctx = *m_ctx.get();
}

void LightComponentSerializer::deserialize() {}

} // namespace astralix
