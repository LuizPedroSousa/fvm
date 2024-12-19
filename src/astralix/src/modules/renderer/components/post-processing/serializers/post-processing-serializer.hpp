#pragma once

#include "serializer.hpp"
#include "json/json.h"

#include "components/post-processing/post-processing-component.hpp"

#include "ecs/components/serializers/component-serializer.hpp"

namespace astralix {
  class PostProcessingComponent;
  class PostProcessingComponentSerializer : public ComponentSerializer {

  public:
    PostProcessingComponentSerializer(
      PostProcessingComponent* post_processing_component);
    PostProcessingComponentSerializer();

    Json::Value parse() override;
    void serialize() override;
    void deserialize() override;
  };
} // namespace astralix