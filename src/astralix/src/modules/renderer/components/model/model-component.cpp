#include "components/material/material-component.hpp"
#include "engine.hpp"

#include "components/mesh/mesh-component.hpp"
#include "components/model/model-component.hpp"
#include "entities/object.hpp"

namespace astralix {

ModelComponent::ModelComponent(COMPONENT_INIT_PARAMS)
    : COMPONENT_INIT(ModelComponent) {}

Model *ModelComponent::attach_model(ResourceID id) {
  auto resource_manager = Engine::get()->get_resource_manager();

  auto model = resource_manager->get_model_by_id(id);

  auto owner = Engine::get()->get_owner(get_owner_id());

  owner->get_component<MeshComponent>()->attach_meshes(model->meshes);
  owner->get_component<MaterialComponent>()->attach_materials(model->materials);

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