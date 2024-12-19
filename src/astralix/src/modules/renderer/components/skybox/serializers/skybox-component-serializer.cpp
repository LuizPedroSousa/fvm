#include "components/skybox/serializers/skybox-component-serializer.hpp"
#include "components/skybox/skybox-component.hpp"
#include "filesystem"
#include "json/json.h"

namespace astralix {

  SkyboxComponentSerializer::SkyboxComponentSerializer(
    SkyboxComponent* skybox_component)
    : ComponentSerializer(skybox_component) {
  }

  SkyboxComponentSerializer::SkyboxComponentSerializer() {}

  Json::Value SkyboxComponentSerializer::parse() {
    ComponentSerializer::parse();

    Json::Value root = ComponentSerializer::parse();

    return root;
  }

  void SkyboxComponentSerializer::serialize() {}

  void SkyboxComponentSerializer::deserialize() {}

} // namespace astralix
