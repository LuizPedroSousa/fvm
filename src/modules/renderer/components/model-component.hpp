#pragma once
#include "assimp/scene.h"
#include "base/component.hpp"
#include "filesystem"
#include "resources/model.hpp"
#include "vector"

class ModelComponent : public Component<ModelComponent> {
  public:
  ModelComponent(COMPONENT_INIT_PARAMS);

  Model *attach_model(ResourceID id);
  std::vector<Model *> attach_models(std::initializer_list<ResourceID> ids);

  private:
  std::vector<ResourceID> m_models;
};