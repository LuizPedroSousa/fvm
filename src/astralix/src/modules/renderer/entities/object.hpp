#pragma once
#include "ecs/entities/entity.hpp"
#include "ecs/entities/ientity.hpp"
#include "glm/glm.hpp"

namespace astralix {

struct ObjectDTO {
  ENTITY_PARAMS
  glm::vec3 position = glm::vec3(0.0f);
  glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
};

class Object : public Entity<Object> {
public:
  Object(ENTITY_INIT_PARAMS, glm::vec3 position = glm::vec3(0.0f),
         glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));

  Object(ObjectDTO payload);

  Object() {};

  void start();
  void pre_update();
  void update();
  void post_update();

  void on_enable() override {};
  void on_disable() override {};
};

} // namespace astralix
