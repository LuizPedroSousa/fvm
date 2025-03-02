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

  Json::Value LightComponentSerializer::serialize() {
    ComponentSerializer::serialize();

    Json::Value root = ComponentSerializer::serialize();

    return root;
  }

  void LightComponentSerializer::save() {}

  void LightComponentSerializer::deserialize() {}

} // namespace astralix
