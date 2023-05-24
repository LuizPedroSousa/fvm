#include "scene-system.hpp"
#include "systems/base/system.hpp"

class RenderSystem : public System<RenderSystem> {
  public:
  RenderSystem(SceneSystem *scene_system);

  Either<BaseException, Unit> start() override;
  void fixed_update(double fixed_dt) override;
  void pre_update(double dt) override;
  void update(double dt) override;
  void post_update(double dt) override;

  private:
  SceneSystem *m_scene_system;
};