#pragma once
#include "components/component.hpp"

namespace astralix {

class TagComponent : public Component<TagComponent> {
public:
  TagComponent(COMPONENT_INIT_PARAMS, std::vector<std::string> tags)
      : tags(tags) {};

  std::vector<std::string> tags;
};

} // namespace astralix
