#pragma once
#include "assimp/scene.h"
#include "ecs/components/component.hpp"
#include "filesystem"
#include "resources/model.hpp"
#include "vector"

namespace astralix {

  class ModelComponent : public Component<ModelComponent>
  {
  public:
    ModelComponent(COMPONENT_INIT_PARAMS);

    Model* attach_model(ResourceID id);
    std::vector<Model*> attach_models(std::initializer_list<ResourceID> ids);

  private:
    std::vector<ResourceID> m_models;
  };

} // namespace astralix