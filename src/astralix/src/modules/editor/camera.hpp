#pragma once
#include "ecs/entities/entity.hpp"
#include "ecs/entities/ientity.hpp"
#include "entities/object.hpp"
#include "glm/glm.hpp"

namespace astralix {

class EditorCamera : public Object {
public:
  EditorCamera(ENTITY_INIT_PARAMS, glm::vec3 position = glm::vec3(0.0f));

  void start();
  void pre_update();
  void update() override;
  void post_update();

  void on_enable() override {};
  void on_disable() override {};
};

} // namespace astralix
