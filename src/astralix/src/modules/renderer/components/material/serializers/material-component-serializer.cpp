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

  Json::Value MaterialComponentSerializer::serialize() {
    ComponentSerializer::serialize();

    Json::Value root = ComponentSerializer::serialize();

    return root;
  }

  void MaterialComponentSerializer::save() {}
  void MaterialComponentSerializer::deserialize() {}

} // namespace astralix