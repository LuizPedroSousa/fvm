#pragma once
#include "ecs/systems/system.hpp"

namespace astralix {

  class LayerSystem : public System<LayerSystem> {
  public:
    LayerSystem();
    ~LayerSystem();

    void start() override;
    void fixed_update(double fixed_dt) override;
    void pre_update(double dt) override;
    void update(double dt) override;
  };

} // namespace astralix