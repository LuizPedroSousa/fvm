#include "components/resource/serializers/resource-component-serializer.hpp"
#include "components/resource/resource-component.hpp"
#include "filesystem"
#include "json/json.h"

namespace astralix {

  ResourceComponentSerializer::ResourceComponentSerializer(
    ResourceComponent* resource_component)
    : ComponentSerializer(resource_component) {
  }

  ResourceComponentSerializer::ResourceComponentSerializer() {}

  Json::Value ResourceComponentSerializer::parse() {
    ComponentSerializer::parse();

    Json::Value root = ComponentSerializer::parse();

    return root;
  }

  void ResourceComponentSerializer::serialize() {}

  void ResourceComponentSerializer::deserialize() {}

} // namespace astralix
