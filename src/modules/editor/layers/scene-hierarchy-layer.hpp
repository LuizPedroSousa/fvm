#pragma once
#include "entities/layer.hpp"
#include "entities/object.hpp"

namespace astralix {
class SceneHierarchyLayer : public Layer {
public:
  SceneHierarchyLayer() : Layer("SceneHierarchy") {}

  void start() override;
  void update() override;
};

} // namespace astralix