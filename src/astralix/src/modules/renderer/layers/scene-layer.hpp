#pragma once
#include "components/camera/camera-component.hpp"
#include "components/light/light-component.hpp"
#include "ecs/entities/entity.hpp"
#include "entities/layer.hpp"
#include "entities/object.hpp"

namespace astralix {

class SceneLayer : public Layer {
public:
  SceneLayer(ENTITY_INIT_PARAMS) : Layer(id, component_manager) {}

  void start() override;
  void update() override;

  void on_enable() override{};
  void on_disable() override{};

private:
  void edit_camera(CameraComponent *camera);
  void edit_entity(IEntity *entity);
};

} // namespace astralix