#pragma once
#include "assimp/scene.h"
#include "base.hpp"
#include "ecs/guid.hpp"
#include "memory"
#include "resources/material.hpp"
#include "resources/model.hpp"
#include "resources/shader.hpp"
#include "resources/texture.hpp"
#include "unordered_map"

namespace astralix {

struct LoadShaderDTO {
  ResourceID id;
  const char *vertex_filename;
  const char *geometry_filename;
  const char *fragment_filename;

  LoadShaderDTO(ResourceID id, const char *vertex_filename,
                const char *fragment_filename,
                const char *geometry_filename = NULL)
      : id(id), vertex_filename(vertex_filename),
        geometry_filename(geometry_filename),
        fragment_filename(fragment_filename){};
};

class ResourceManager {
public:
  Texture *load_texture(ResourceID id, std::string name, const char *filename);

  Texture *load_cubemap(ResourceID id, std::string name,
                        std::vector<std::string> faces);

  Shader *load_shader(LoadShaderDTO dto);

  void load_shaders(std::initializer_list<LoadShaderDTO> shaders);

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

private:
  std::unordered_map<ResourceID, Scope<Texture>> m_texture_table;
  std::unordered_map<ResourceID, Scope<Shader>> m_shader_table;
  std::unordered_map<ResourceID, Scope<Model>> m_model_table;
};
} // namespace astralix