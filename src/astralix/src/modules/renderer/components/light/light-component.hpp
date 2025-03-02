#pragma once
#include "base.hpp"
#include "components/camera/camera-component.hpp"
#include "ecs/entities/ientity.hpp"
#include "ecs/guid.hpp"
#include "either.hpp"
#include "entities/object.hpp"
#include "glm/glm.hpp"
#include "light-component.hpp"
#include "strategies/strategy.hpp"

namespace astralix {

class SceneSystem;

class LightComponent : public Component<LightComponent> {
public:
  LightComponent(COMPONENT_INIT_PARAMS, Scope<LightStrategy> strategy,
                 EntityID camera);

  void start();
  void update(IEntity *source, Object *object);

private:
  EntityID m_camera;

  Scope<LightStrategy> m_strategy;
};
} // namespace astralix
