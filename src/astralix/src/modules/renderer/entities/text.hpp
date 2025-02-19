#include "ecs/entities/entity.hpp"
#include "ecs/guid.hpp"
#include "glm/glm.hpp"
#include "resources/mesh.hpp"
#include <imgui.h>

namespace astralix {
class Text : public Entity<Text> {
public:
  Text(ENTITY_INIT_PARAMS, std::string text, glm::vec2 position, float scale,
       glm::vec3 color);

  void start();
  void pre_update();
  void update();
  void post_update();

  void on_enable() override {};
  void on_disable() override {};

private:
  ResourceID m_cubemap_id;
  ResourceID m_shader_id;
  std::string m_text;

  glm::vec2 m_position;
  float m_scale;
  glm::vec3 m_color;

  Mesh m_mesh;
};

} // namespace astralix
