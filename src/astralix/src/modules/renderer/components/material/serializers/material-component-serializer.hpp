#pragma once

#include "serializer.hpp"
#include "json/json.h"

#include "components/material/material-component.hpp"

#include "components/serializers/component-serializer.hpp"

namespace astralix {
  class MaterialComponent;
  class MaterialComponentSerializer : public ComponentSerializer {

  public:
    MaterialComponentSerializer(MaterialComponent* material_component);
    MaterialComponentSerializer();

    void serialize() override;
    void deserialize() override;
  };
} // namespace astralix
