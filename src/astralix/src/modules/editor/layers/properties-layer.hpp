#pragma once

#include "entities/ientity.hpp"
#include "entities/layer.hpp"
#include "entities/object.hpp"

namespace astralix {

  class PropertiesLayer : public Layer {
  public:
    PropertiesLayer() : Layer("Properties") {}

    void start() override;
    void update() override;

  private:
    template <typename T, typename UIFunction>
    static void draw_component(IEntity* entity, const char* icon_name,
      UIFunction uiFunction);

    void draw_components(IEntity* entity);
  };

} // namespace astralix