#pragma once
#include "components/camera-component.hpp"
#include "either.hpp"
#include "entities/object.hpp"
#include "exceptions/base-exception.hpp"
#include "glm/glm.hpp"
#include "light-component.hpp"
#include "uniform.hpp"

class RenderComponent;

class LightComponent : public Component<LightComponent> {
  public:
  LightComponent(COMPONENT_INIT_PARAMS, CameraComponent *camera);
  LightComponent(){};

  friend class RenderComponent;

  private:
  void start(Uniform *uniform);
  void update(Uniform *uniform);

  CameraComponent *m_camera;
};