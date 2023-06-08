#pragma once
#include "entities/base/entity.hpp"
#include "glm/glm.hpp"
#include "uniform.hpp"

class RenderComponent;

class Object : public Entity<Object> {
  public:
  Object(ENTITY_INIT_PARAMS, glm::vec3 position, glm::vec3 scale = glm::vec3(0.5f, 0.5f, 0.5f));
  Object(){};

  // static Vertices cube();

  void on_enable() override{};
  void on_disable() override{};

  friend class RenderComponent;
};
