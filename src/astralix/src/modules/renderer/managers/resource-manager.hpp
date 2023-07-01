#pragma once
#include "assimp/scene.h"
#include "base-manager.hpp"
#include "base.hpp"
#include "ecs/guid.hpp"
#include "memory"
#include "resources/material.hpp"
#include "resources/model.hpp"
#include "resources/shader.hpp"
#include "resources/texture.hpp"
#include "unordered_map"

namespace astralix {

class ResourceManager : public BaseManager<ResourceManager> {
public:
  Texture *load_texture(CreateTextureDTO);
  void load_textures(std::initializer_list<CreateTextureDTO>);

  Texture *load_cubemap(ResourceID id, std::vector<std::string> faces);

  Shader *load_shader(CreateShaderDTO dto);

  void load_shaders(std::initializer_list<CreateShaderDTO> shaders);

  Material *load_material(ResourceID id, aiMaterial *ai_material);

  Model *load_model(ResourceID id, const char *filename);
  void load_models(
      std::initializer_list<std::pair<ResourceID, const char *>> models);

  Shader *get_shader_by_id(ResourceID id);
  Texture *get_texture_by_id(ResourceID id);
  Texture *get_cubemap_by_id(ResourceID id);
  Model *get_model_by_id(ResourceID id);
  std::vector<Model *> get_models_by_ids(std::initializer_list<ResourceID> ids);
  Material *get_material_by_id(ResourceID id);

  std::unordered_map<ResourceID, Scope<Material>> m_material_table;

  ResourceManager() = default;

private:
  std::unordered_map<ResourceID, Scope<Texture>> m_texture_table;
  std::unordered_map<ResourceID, Scope<Shader>> m_shader_table;
  std::unordered_map<ResourceID, Scope<Model>> m_model_table;
};
} // namespace astralix