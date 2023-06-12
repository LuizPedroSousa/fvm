#pragma once
#include "ecs/entities/entity.hpp"
#include "glm/glm.hpp"
#include "uniform.hpp"

namespace astralix {

class Object : public Entity<Object> {
public:
  Object(ENTITY_INIT_PARAMS, glm::vec3 position,
         glm::vec3 scale = glm::vec3(0.5f, 0.5f, 0.5f));
  Object(){};

  void on_enable() override{};
  void on_disable() override{};
};

} // namespace astralix
