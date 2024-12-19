#include "components/post-processing/serializers/post-processing-serializer.hpp"
#include "components/post-processing/post-processing-component.hpp"
#include "filesystem"
#include "json/json.h"

namespace astralix {

  PostProcessingComponentSerializer::PostProcessingComponentSerializer(
    PostProcessingComponent* post_processing_component)
    : ComponentSerializer(post_processing_component) {
  }

  PostProcessingComponentSerializer::PostProcessingComponentSerializer() {}

  Json::Value PostProcessingComponentSerializer::parse() {
    ComponentSerializer::parse();

    Json::Value root = ComponentSerializer::parse();

    return root;
  }

  void PostProcessingComponentSerializer::serialize() {}

  void PostProcessingComponentSerializer::deserialize() {}

} // namespace astralix
