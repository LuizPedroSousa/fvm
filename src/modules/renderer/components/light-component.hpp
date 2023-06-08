#pragma once
#include "components/camera-component.hpp"
#include "either.hpp"
#include "entities/object.hpp"
#include "exceptions/base-exception.hpp"
#include "glm/glm.hpp"
#include "light-component.hpp"
#include "uniform.hpp"
#include "utils/guid.hpp"

class RenderSystem;

enum LightType {
  Directional,
  Point,
  Spotlight,
};

struct Light {
  LightType type;
};

class LightComponent : public Component<LightComponent> {
  public:
  LightComponent(COMPONENT_INIT_PARAMS, CameraComponent *camera);
  LightComponent(){};

  friend class RenderSystem;

  void add_source(glm::vec3 position, ResourceID shader_id);

  void use_directional();
  void use_spotlight();
  void use_point();

  private:
  void start();
  void update(Object *object);

  CameraComponent *m_camera;

  Light m_light;

  Object *m_source;
};