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
  Ref<Texture> load_texture(Ref<Texture> texture);
  void load_textures(std::initializer_list<Ref<Texture>> textures);

  Ref<Shader> load_shader(Ref<Shader> shader);

  void load_shaders(std::initializer_list<Ref<Shader>> shaders);

  // Material *load_material(ResourceID id, aiMaterial *ai_material);

  Ref<Model> load_model(Ref<Model> model);
  void load_models(std::initializer_list<Ref<Model>> models);

  Ref<Shader> get_shader_by_id(ResourceID id);
  Ref<Texture> get_texture_by_id(ResourceID id);

  Model *get_model_by_id(ResourceID id);
  std::vector<Model *> get_models_by_ids(std::initializer_list<ResourceID> ids);
  Ref<Material> get_material_by_id(ResourceID id);

  Ref<Material> load_material(Ref<Material> material);

  ResourceManager() = default;

private:
  std::unordered_map<ResourceID, Ref<Texture>> m_texture_table;
  std::unordered_map<ResourceID, Ref<Shader>> m_shader_table;
  std::unordered_map<ResourceID, Ref<Model>> m_model_table;
  std::unordered_map<ResourceID, Ref<Material>> m_material_table;
};
} // namespace astralix
