#include "components/light/serializers/light-component-serializer.hpp"
#include "components/light/light-component.hpp"
#include "filesystem"
#include "json/json.h"

namespace astralix {

  LightComponentSerializer::LightComponentSerializer(
    LightComponent* light_component)
    : ComponentSerializer(light_component) {
  }

  LightComponentSerializer::LightComponentSerializer() {}

  Json::Value LightComponentSerializer::parse() {
    ComponentSerializer::parse();

    Json::Value root = ComponentSerializer::parse();

    return root;
  }

  void LightComponentSerializer::serialize() {}

  void LightComponentSerializer::deserialize() {}

} // namespace astralix
