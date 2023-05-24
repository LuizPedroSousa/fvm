#pragma once
#include "entities/layer.hpp"
#include "entities/object.hpp"
#include "entities/base/entity.hpp"

class ObjectListLayer : public Entity<ObjectListLayer>, public Layer {
  public:
  ObjectListLayer(ENTITY_INIT_PARAMS) : ENTITY_INIT() {}

  void start() override;
  void update() override;

  void on_enable() override{};
  void on_disable() override{};

  private:
  void edit_object(Object *object);
};