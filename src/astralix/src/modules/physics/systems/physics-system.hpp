#include "ecs/systems/system.hpp"

namespace astralix {

class PhysicsSystem : public System<PhysicsSystem> {
public:
  PhysicsSystem();

  void start() override;
  void fixed_update(double fixed_dt) override;
  void pre_update(double dt) override;
  void update(double dt) override;
  void post_update(double dt) override;
};

} // namespace astralix