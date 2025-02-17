#include "resource-manager.hpp"
#include "glad/glad.h"

namespace astralix {
Ref<Texture> ResourceManager::load_texture(Ref<Texture> texture) {
  auto texture_id = texture->get_resource_id();
  auto inserted_texture =
      m_texture_table.emplace(texture_id, std::move(texture));

  ASTRA_ASSERT_THROW(!inserted_texture.second, "can't insert texture");

  return m_texture_table[texture_id];
}

void ResourceManager::load_textures(
    std::initializer_list<Ref<Texture>> textures) {
  for (auto texture : textures) {
    load_texture(texture);
  }
}

Ref<Shader> ResourceManager::load_shader(Ref<Shader> shader) {
  auto shader_id = shader->get_resource_id();

  shader->attach();

  auto inserted_shader = m_shader_table.emplace(shader_id, std::move(shader));

  ASTRA_ASSERT_THROW(!inserted_shader.second, "can't insert shader");

  return m_shader_table[shader_id];
}

void ResourceManager::load_shaders(std::initializer_list<Ref<Shader>> shaders) {
  for (auto shader : shaders) {
    load_shader(shader);
  }
}

Ref<Model> ResourceManager::load_model(Ref<Model> model) {
  auto model_id = model->get_resource_id();

  auto inserted_model = m_model_table.emplace(model_id, std::move(model));

  ASTRA_ASSERT_THROW(!inserted_model.second, "can't insert model");

  return m_model_table[model_id];
}

void ResourceManager::load_models(std::initializer_list<Ref<Model>> models) {
  for (auto &model : models) {
    load_model(model);
  }
}

Ref<Shader> ResourceManager::get_shader_by_id(ResourceID id) {
  auto it = m_shader_table.find(id);

  if (it != m_shader_table.end()) {
    return it->second;
  }

  return nullptr;
}

Ref<Texture> ResourceManager::get_texture_by_id(ResourceID id) {
  auto it = m_texture_table.find(id);

  if (it != m_texture_table.end()) {
    return it->second;
  }

  return nullptr;
}

Model *ResourceManager::get_model_by_id(ResourceID id) {
  auto it = m_model_table.find(id);

  if (it != m_model_table.end()) {
    return it->second.get();
  }

  return nullptr;
}

std::vector<Model *>
ResourceManager::get_models_by_ids(std::initializer_list<ResourceID> ids) {
  std::vector<Model *> models;

  for (auto id : ids) {
    auto model = get_model_by_id(id);

    if (model != nullptr) {
      models.push_back(model);
    }
  }

  return models;
}

Ref<Material> ResourceManager::load_material(Ref<Material> material) {
  auto material_id = material->get_resource_id();

  auto inserted_material =
      m_material_table.emplace(material_id, std::move(material));

  ASTRA_ASSERT_THROW(!inserted_material.second, "Can't insert material");

  return m_material_table[material_id];
}

Ref<Material> ResourceManager::get_material_by_id(ResourceID id) {
  auto it = m_material_table.find(id);

  if (it != m_material_table.end()) {
    return it->second;
  }

  return nullptr;
}

} // namespace astralix
