#include "resource-manager.hpp"
#include "engine.hpp"
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

Shader *ResourceManager::load_shader(CreateShaderDTO dto) {
  auto shader_exists = get_shader_by_id(dto.id);

  if (shader_exists != nullptr) {
    return shader_exists;
  }

  auto created_shader = Shader::create(dto);

  ASTRA_ASSERT_EITHER_THROW(created_shader);

  Scope<Shader> shader_ptr = create_scope<Shader>(created_shader.right());

  auto inserted_shader = m_shader_table.emplace(dto.id, std::move(shader_ptr));

  ASTRA_ASSERT_THROW(!inserted_shader.second, "can't insert shader");

  return shader_ptr.get();
}

void ResourceManager::load_shaders(
    std::initializer_list<CreateShaderDTO> dtos) {
  for (auto &dto : dtos) {
    load_shader(dto);
  }
}

Material *ResourceManager::load_material(ResourceID material_id,
                                         aiMaterial *ai_material) {
  auto material_exists = get_material_by_id(material_id);

  if (material_exists != nullptr) {
    return material_exists;
  }

  auto get_texture = [&](aiTextureType type) {
    std::vector<ResourceID> textures;
    for (unsigned int i = 0; i < ai_material->GetTextureCount(type); i++) {
      aiString filename_str;
      ai_material->GetTexture(type, i, &filename_str);

      const char *filename = filename_str.C_Str();

      ResourceID texture_id = "textures::" + material_id + "::" + filename;

      auto get_name = [type]() -> std::string {
        std::string prefix = "materials[0].";

        std::string type_str =
            type == aiTextureType_DIFFUSE ? "diffuse" : "specular";

        std::string name = prefix + type_str;

        return name;
      };

      std::string path = "models/" + std::string(filename);
      Ref<Texture> texture_ptr =
          load_texture(Texture2D::create(texture_id, path));

      textures.push_back(texture_ptr->get_resource_id());
    };

    return textures;
  };

  auto diffuses = get_texture(aiTextureType_DIFFUSE);
  auto speculars = get_texture(aiTextureType_SPECULAR);

  auto created_material = Material::create(material_id, diffuses, speculars);

  Scope<Material> material_ptr = create_scope<Material>(created_material);

  auto inserted_material =
      m_material_table.emplace(material_id, std::move(material_ptr));

  ASTRA_ASSERT_THROW(!inserted_material.second, "Can't insert material");

  return m_material_table[material_id].get();
}

Model *ResourceManager::load_model(ResourceID id, const char *filename) {
  auto model_exists = get_model_by_id(id);

  if (model_exists != nullptr) {
    return model_exists;
  }

  auto model = Model::create(id, filename);

  ASTRA_ASSERT_EITHER_THROW(model);

  Scope<Model> model_ptr = create_scope<Model>(model.right());

  auto inserted_moel = m_model_table.emplace(id, std::move(model_ptr));

  ASTRA_ASSERT_THROW(!inserted_moel.second, "Can't insert model");

  return model_ptr.get();
}

void ResourceManager::load_models(
    std::initializer_list<std::pair<ResourceID, const char *>> models) {
  for (auto &model : models) {
    load_model(model.first, model.second);
  }
}

Shader *ResourceManager::get_shader_by_id(ResourceID id) {
  auto it = m_shader_table.find(id);

  if (it != m_shader_table.end()) {
    return it->second.get();
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

Material *ResourceManager::get_material_by_id(ResourceID id) {
  auto it = m_material_table.find(id);

  if (it != m_material_table.end()) {
    return it->second.get();
  }

  return nullptr;
}

} // namespace astralix
