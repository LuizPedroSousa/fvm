#include "systems/system.hpp"

namespace astralix {

class HDRSystem : public System<HDRSystem> {
public:
  HDRSystem();
  ~HDRSystem();

  void start() override;
  void fixed_update(double fixed_dt) override;
  void pre_update(double dt) override;
  void update(double dt) override;
};

} // namespace astralix
