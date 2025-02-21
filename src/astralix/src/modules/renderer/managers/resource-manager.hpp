#pragma once
#include "base-manager.hpp"
#include "base.hpp"
#include "ecs/guid.hpp"
#include "resources/font.hpp"
#include "resources/material.hpp"
#include "resources/model.hpp"
#include "resources/shader.hpp"
#include "resources/texture.hpp"
#include "unordered_map"
#include <initializer_list>

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

  void load_fonts(std::initializer_list<Ref<Font>> fonts);

  Ref<Shader> get_shader_by_id(ResourceID id);
  Ref<Texture> get_texture_by_id(ResourceID id);

  Model *get_model_by_id(ResourceID id);
  std::vector<Model *> get_models_by_ids(std::initializer_list<ResourceID> ids);
  Ref<Material> get_material_by_id(ResourceID id);
  Ref<Font> get_font_by_id(ResourceID id);

  Ref<Material> load_material(Ref<Material> material);
  void load_materials(std::initializer_list<Ref<Material>> materials);
  Ref<Font> load_font(Ref<Font> font);

  ResourceManager() = default;

private:
  std::unordered_map<ResourceID, Ref<Texture>> m_texture_table;
  std::unordered_map<ResourceID, Ref<Shader>> m_shader_table;
  std::unordered_map<ResourceID, Ref<Model>> m_model_table;
  std::unordered_map<ResourceID, Ref<Material>> m_material_table;
  std::unordered_map<ResourceID, Ref<Font>> m_font_table;
};
} // namespace astralix
