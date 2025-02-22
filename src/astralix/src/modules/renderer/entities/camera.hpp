#pragma once
#include "components/transform/transform-component.hpp"
#include "ecs/entities/entity.hpp"
#include "ecs/entities/ientity.hpp"
#include "entities/object.hpp"
#include "glm/glm.hpp"

namespace astralix {

enum CameraMode { Free = 0, FirstPerson = 1, ThirdPerson = 2, Orbital = 3 };

class Camera : public Entity<Camera> {

public:
  Camera(ENTITY_INIT_PARAMS, CameraMode mode,
         glm::vec3 position = glm::vec3(0.0f));

  void update();

  void on_enable() override {}
  void on_disable() override {}

private:
  CameraMode m_mode;

  float m_yaw = -90.0f;
  float m_pitch = 0.0f;

  float m_speed = 4.0f;
  float m_sensitivity = 0.1f;

  TransformComponent *target = nullptr;
};

} // namespace astralix
