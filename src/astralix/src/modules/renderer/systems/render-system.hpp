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
  void post_update(double dt) override;

  glm::vec4 *get_clear_color() { return &m_clear_color; };

private:
  glm::vec4 m_clear_color;
};

} // namespace astralix