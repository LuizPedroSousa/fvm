#include "managers/layer-manager.hpp"
#include "systems/base/system.hpp"

class LayerSystem : public System<LayerSystem> {
  public:
  LayerSystem() {
    LayerManager::init();
  };

  Either<BaseException, Unit> start() override;
  void fixed_update(double fixed_dt) override;
  void pre_update(double dt) override;
  void update(double dt) override;
  void post_update(double dt) override;
};