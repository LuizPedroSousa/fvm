#include "components/material/serializers/material-component-serializer.hpp"
#include "components/material/material-component.hpp"
#include "filesystem"
#include "json/json.h"

namespace astralix {

MaterialComponentSerializer::MaterialComponentSerializer(
    MaterialComponent *material_component)
    : ComponentSerializer(material_component) {}

MaterialComponentSerializer::MaterialComponentSerializer() {}

void MaterialComponentSerializer::serialize() {
  ComponentSerializer::serialize();

  SerializationContext &ctx = *m_ctx.get();
}

void MaterialComponentSerializer::deserialize() {}

} // namespace astralix
