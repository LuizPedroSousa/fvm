#pragma once
#include "assimp/scene.h"
#include "memory"
#include "resources/material.hpp"
#include "resources/model.hpp"
#include "resources/shader.hpp"
#include "resources/texture.hpp"
#include "unordered_map"
#include "utils/guid.hpp"

class ResourceManager {
public:
  Texture *load_texture(ResourceID id, std::string name, int color_type,
                        const char *filename);
  Shader *load_shader(ResourceID id, const char *vertex_filename,
                      const char *fragment_filename);
  void load_shaders(
      std::initializer_list<std::tuple<ResourceID, const char *, const char *>>
          shaders);

  Material *load_material(ResourceID id, aiMaterial *ai_material);

  Model *load_model(ResourceID id, const char *filename);
  void load_models(
      std::initializer_list<std::pair<ResourceID, const char *>> models);

  Shader *get_shader_by_id(ResourceID id);
  Texture *get_texture_by_id(ResourceID id);
  Model *get_model_by_id(ResourceID id);
  std::vector<Model *> get_models_by_ids(std::initializer_list<ResourceID> ids);
  Material *get_material_by_id(ResourceID id);

  std::unordered_map<ResourceID, std::unique_ptr<Material>> m_material_table;

private:
  std::unordered_map<ResourceID, std::unique_ptr<Texture>> m_texture_table;
  std::unordered_map<ResourceID, std::unique_ptr<Shader>> m_shader_table;
  std::unordered_map<ResourceID, std::unique_ptr<Model>> m_model_table;
};
