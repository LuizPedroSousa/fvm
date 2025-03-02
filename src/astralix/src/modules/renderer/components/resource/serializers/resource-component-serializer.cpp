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

  Json::Value ResourceComponentSerializer::serialize() {
    ComponentSerializer::serialize();

    Json::Value root = ComponentSerializer::serialize();

    return root;
  }

  void ResourceComponentSerializer::save() {}

  void ResourceComponentSerializer::deserialize() {}

} // namespace astralix
