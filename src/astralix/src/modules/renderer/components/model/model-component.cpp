#include "components/material/material-component.hpp"

#include "components/mesh/mesh-component.hpp"
#include "components/model/model-component.hpp"
#include "entities/object.hpp"

namespace astralix {

ModelComponent::ModelComponent(COMPONENT_INIT_PARAMS)
    : COMPONENT_INIT(ModelComponent, "Model Renderer", true) {}

Model *ModelComponent::attach_model(ResourceID id) {
  auto resource_manager = ResourceManager::get();

  auto model = resource_manager->get_model_by_id(id);

  auto owner = get_owner();

  owner->get_or_add_component<MeshComponent>()->attach_meshes(model->meshes);
  owner->get_or_add_component<MaterialComponent>()->attach_materials(
      model->materials);

  m_models.push_back(id);

  return model;
};

std::vector<Model *>
ModelComponent::attach_models(std::initializer_list<ResourceID> ids) {

  std::vector<Model *> models;

  for (auto &id : ids) {
    auto model = attach_model(id);
    models.push_back(model);
  }

  return models;
};

} // namespace astralix