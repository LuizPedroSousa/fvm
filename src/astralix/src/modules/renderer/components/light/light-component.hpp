#pragma once
#include "components/camera/camera-component.hpp"
#include "ecs/guid.hpp"
#include "either.hpp"
#include "entities/light-source.hpp"
#include "entities/object.hpp"
#include "exceptions/base-exception.hpp"
#include "glm/glm.hpp"
#include "light-component.hpp"
#include "strategies/strategy.hpp"
#include "uniform.hpp"

namespace astralix {

class SceneSystem;

class LightComponent : public Component<LightComponent> {
public:
  LightComponent(COMPONENT_INIT_PARAMS, std::unique_ptr<LightStrategy> strategy,
                 CameraComponent *camera);
  LightComponent(){};

  friend class SceneSystem;

  void add_source(glm::vec3 position, ResourceID shader_id);

private:
  void start();
  void update(Object *object);

  CameraComponent *m_camera;

  LightSource *m_source;
  std::unique_ptr<LightStrategy> m_strategy;
};
} // namespace astralix