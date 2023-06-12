#pragma once
#include "ecs/entities/ientity.hpp"
#include "ecs/systems/system.hpp"
#include "entities/scene.hpp"

#include "uniform.hpp"

namespace astralix {

class RenderSystem;

class SceneSystem : public System<SceneSystem> {

public:
  void start() override;
  void fixed_update(double fixed_dt) override;
  void pre_update(double dt) override;
  void update(double dt) override;
  void post_update(double dt) override;

  // this guys are called by the render system after an object draw
  void pre_render(Uniform *uniform);
  // ----------

  friend class RenderSystem;

private:
  Scene *get_current_scene();

  int m_current_scene = 0;
};

} // namespace astralix
