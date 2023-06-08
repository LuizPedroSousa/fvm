#include "components/material-component.hpp"
#include "game.hpp"

#include "components/mesh-component.hpp"
#include "entities/object.hpp"
#include "model-component.hpp"

ModelComponent::ModelComponent(COMPONENT_INIT_PARAMS) : COMPONENT_INIT(ModelComponent) {
}

Model *ModelComponent::attach_model(ResourceID id) {
  auto resource_manager = Game::get()->get_resource_manager();

  auto model = resource_manager->get_model_by_id(id);

  auto owner = Game::get()->get_owner(get_owner_id());

  owner->get_component<MeshComponent>()->attach_meshes(model->meshes);
  owner->get_component<MaterialComponent>()->attach_materials(model->materials);

  m_models.push_back(id);

  return model;
};

std::vector<Model *> ModelComponent::attach_models(std::initializer_list<ResourceID> ids) {

  std::vector<Model *> models;

  for (auto &id : ids) {
    auto model = attach_model(id);
    models.push_back(model);
  }

  return models;
};
