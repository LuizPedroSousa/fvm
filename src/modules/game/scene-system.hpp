#pragma once
#include "entities/base/ientity.hpp"
#include "scene.hpp"
#include "systems/base/system.hpp"

#include "uniform.hpp"

class RenderSystem;

class SceneSystem : public System<SceneSystem> {

  public:
  Either<BaseException, Unit> start() override;
  void fixed_update(double fixed_dt) override;
  void pre_update(double dt) override;
  void update(double dt) override;
  void post_update(double dt) override;

  // this guys are called by the render system after an object draw
  void post_render(Uniform *uniform);
  // ----------

  friend class RenderSystem;

  private:
  IEntity *get_current_scene();

  std::vector<EntityID> m_scenes;
  int m_current_scene = 0;
};