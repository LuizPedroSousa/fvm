#pragma once
#include "entities/base/entity.hpp"
#include "glm/glm.hpp"
#include "uniform.hpp"

struct Vertices {
  float *vertex;
  size_t size;
};

struct Buffers {
  u_int VAO;
  u_int VBO;
};

class RenderComponent;

class Object : public Entity<Object> {
  public:
  Object(ENTITY_INIT_PARAMS, Vertices vertices, glm::vec3 position);
  Object(){};

  static Vertices cube();

  void on_enable() override{};
  void on_disable() override{};

  void start();
  void update();

  Vertices vertices;

  friend class RenderComponent;

  private:
  Buffers m_buffers;
  Uniform *m_uniform;
};
