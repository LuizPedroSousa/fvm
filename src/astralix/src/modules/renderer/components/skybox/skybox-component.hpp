#pragma once
#include "ecs/components/component.hpp"

namespace astralix {

class SkyboxComponent : public Component<SkyboxComponent> {
public:
  SkyboxComponent(COMPONENT_INIT_PARAMS, const ResourceID cubemap_id,
                  const ResourceID skybox_shader_id);

  void start();
  void pre_update();
  void update();
  void post_update();
};

} // namespace astralix