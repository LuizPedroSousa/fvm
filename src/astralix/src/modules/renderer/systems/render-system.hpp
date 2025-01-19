#include "ecs/systems/system.hpp"
#include "glm/glm.hpp"

namespace astralix {

  class RenderSystem : public System<RenderSystem> {
  public:
    RenderSystem();
    ~RenderSystem();

    void start() override;
    void fixed_update(double fixed_dt) override;
    void pre_update(double dt) override;
    void update(double dt) override;
  };

} // namespace astralix