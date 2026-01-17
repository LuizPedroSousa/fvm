#include "systems/system.hpp"

namespace astralix {

  class LightSystem : public System<LightSystem> {
  public:
    LightSystem();
    ~LightSystem();

    void start() override;
    void fixed_update(double fixed_dt) override;
    void pre_update(double dt) override;
    void update(double dt) override;
  };

} // namespace astralix
