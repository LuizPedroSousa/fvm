#pragma once
#include "entities/layer.hpp"
#include "entities/object.hpp"

class ObjectListLayer : public Layer {
public:
  ObjectListLayer(ENTITY_INIT_PARAMS) : Layer(id, component_manager) {}

  void start() override;
  void update() override;

private:
  void edit_object(Object *object);
};