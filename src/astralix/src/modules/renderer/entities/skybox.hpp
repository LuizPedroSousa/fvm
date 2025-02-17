#include "ecs/entities/entity.hpp"
#include "ecs/guid.hpp"
#include <imgui.h>

namespace astralix {
class Skybox : public Entity<Skybox> {
public:
  Skybox(ENTITY_INIT_PARAMS, const ResourceID cubemap_id,
         const ResourceID shader_id);
  Skybox() {};

  void start();
  void pre_update();
  void update();
  void post_update();

  void on_enable() override {};
  void on_disable() override {};

private:
  ResourceID m_cubemap_id;
  ResourceID m_shader_id;
};

} // namespace astralix
