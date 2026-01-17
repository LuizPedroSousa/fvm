#pragma once
#include "base.hpp"
#include "components/camera/camera-component.hpp"
#include "either.hpp"
#include "entities/ientity.hpp"
#include "entities/object.hpp"
#include "glm/glm.hpp"
#include "guid.hpp"
#include "light-component.hpp"
#include "strategies/strategy.hpp"

namespace astralix {

class SceneSystem;

class LightComponent : public Component<LightComponent> {
public:
  LightComponent(COMPONENT_INIT_PARAMS, EntityID camera);

  void start();
  void update(Object *object, size_t &index);

  LightComponent *strategy(Scope<LightStrategy> strategy) {
    m_strategy = std::move(strategy);
    return this;
  }

private:
  EntityID m_camera;

  Scope<LightStrategy> m_strategy;
};
} // namespace astralix
