#pragma once

#include "components/serializers/component-serializer.hpp"

namespace astralix {
class TagComponent;
class TagComponentSerializer : public ComponentSerializer {

public:
  TagComponentSerializer(TagComponent *transform_component);
  TagComponentSerializer();

  void serialize() override;
  void deserialize() override;
};
} // namespace astralix
