#include "components/material/serializers/material-component-serializer.hpp"
#include "components/material/material-component.hpp"
#include "filesystem"
#include "json/json.h"

namespace astralix {

  MaterialComponentSerializer::MaterialComponentSerializer(
    MaterialComponent* material_component)
    : ComponentSerializer(material_component) {
  }

  MaterialComponentSerializer::MaterialComponentSerializer() {}

  Json::Value MaterialComponentSerializer::parse() {
    ComponentSerializer::parse();

    Json::Value root = ComponentSerializer::parse();

    return root;
  }

  void MaterialComponentSerializer::serialize() {}
  void MaterialComponentSerializer::deserialize() {}

} // namespace astralix