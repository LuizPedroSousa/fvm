#include "prologue.hpp"
#include "components/camera/camera-component.hpp"
#include "components/light/light-component.hpp"
#include "components/light/strategies/directional-strategy.hpp"
#include "components/material/material-component.hpp"
#include "components/mesh-collision/mesh-collision-component.hpp"
#include "components/mesh/mesh-component.hpp"
#include "components/resource/resource-component.hpp"
#include "components/rigidbody/rigidbody-component.hpp"
#include "components/transform/transform-component.hpp"
#include "entities/object.hpp"
#include "entities/skybox.hpp"
#include "entities/text.hpp"
#include "glad/glad.h"
#include "glm/fwd.hpp"
#include "managers/resource-manager.hpp"
#include "resources/material.hpp"
#include "resources/model.hpp"
#include <glm/gtx/string_cast.hpp>

Prologue::Prologue() : Scene("prologue") {}

#define ATTACH_MESH(var)                                                       \
  var->add_component<MeshComponent>()->attach_mesh(astralix::Mesh::cube(1.0f));
#define ATTACH_LIGHTING_SHADER(var)                                            \
  var->get_component<ResourceComponent>()->attach_shader("shaders::lighting"); \
  var->get_or_add_component<MaterialComponent>()->attach_material(             \
      "materials::brick")

void Prologue::load_resources() {
  auto manager = ResourceManager::get();

  manager->load_textures(
      ///

      {Texture3D::create("cubemaps::skybox",
                         {
                             "textures/skybox/right.jpg",
                             "textures/skybox/left.jpg",
                             "textures/skybox/top.jpg",
                             "textures/skybox/bottom.jpg",
                             "textures/skybox/front.jpg",
                             "textures/skybox/back.jpg",
                         }),
       ///

       Texture2D::create("textures::block::diffuse", "textures/diffuse.png",
                         false

                         //
                         ),

       Texture2D::create(
           "textures::brick::diffuse", "textures/brickwall.jpg", false,

           {//
            {TextureParameter::WrapS, TextureValue::Linear},
            {TextureParameter::WrapT, TextureValue::Linear},

            {TextureParameter::MagFilter, TextureValue::LinearMipMap},
            {TextureParameter::MinFilter, TextureValue::Linear}

           }

           //
           ),

       Texture2D::create(
           "textures::brick::normal", "textures/brickwall_normal.jpg", false,

           {//
            {TextureParameter::WrapS, TextureValue::Linear},
            {TextureParameter::WrapT, TextureValue::Linear},
            {TextureParameter::MagFilter, TextureValue::LinearMipMap},
            {TextureParameter::MinFilter, TextureValue::Linear}

           }

           )

      });

  manager->load_materials(
      {Material::create("materials::brick", {"textures::brick::diffuse"}, {},
                        "textures::brick::normal")});

  manager->load_materials(
      {Material::create("materials::diffuse", {"textures::block::diffuse"})});

  manager->load_models(
      {Model::create("module::grass::flat", "models/Grass_Flat.obj")});

  manager->load_shaders(
      ///
      {
          Shader::create("shaders::post_processing",
                         "fragment/postprocessing.glsl",
                         "vertex/postprocessing.glsl"),
          Shader::create("shaders::lighting", "fragment/light.glsl",
                         "vertex/light.glsl"),
          Shader::create("shaders::skybox", "fragment/skybox.glsl",
                         "vertex/skybox.glsl"),
          Shader::create("shaders::glyph", "fragment/glyph.glsl",
                         "vertex/glyph.glsl")
          ///
      }

  );

  // manager->load_fonts(
  //     ///
  //     {
  //         Font::create("roboto", "fonts/roboto.ttf"),
  //         ///
  //     });
}

void Prologue::create_tile_grid(int columns, int rows, float tile_size,
                                RigidType type, float y, glm::vec3 scale) {
  float offset_x = (columns * tile_size) / 2.0f - tile_size / 2.0f;
  float offset_z = (rows * tile_size) / 2.0f - tile_size / 2.0f;

  for (int col = 0; col < columns; ++col) {
    for (int row = 0; row < rows; ++row) {
      float x = col * tile_size - offset_x;
      float z = row * tile_size - offset_z;

      auto tile = add_entity<Object>("tile", glm::vec3(x, y, z));

      ATTACH_MESH(tile);
      ATTACH_LIGHTING_SHADER(tile);

      tile->get_component<TransformComponent>()->set_scale(glm::vec3(0.5f));

      tile->get_component<TransformComponent>()->set_scale(scale);

      tile->add_component<MeshCollisionComponent>();
      // tile->add_component<RigidBodyComponent>(type);
    }
  }
}

void Prologue::load_scene_components() {

  auto camera =
      add_entity<astralix::Object>("camera", glm::vec3(0.0f, 4.0f, 0.0f));

  camera->add_component<CameraComponent>();

  auto strategy = astralix::create_scope<astralix::DirectionalStrategy>();

  auto light =
      add_entity<astralix::Object>("Light", glm::vec3(-2.0f, 4.0f, -1.0f));

  // auto light =
  //     add_entity<astralix::Object>("Light", glm::vec3(0.0, 4.0f, 0.3f));

  light->add_component<LightComponent>(std::move(strategy),
                                       camera->get_entity_id());

  add_entity<Skybox>("skybox", "cubemaps::skybox", "shaders::skybox");

  create_tile_grid(6, 6, 1.0f, RigidType::Static);
  create_tile_grid(6, 6, 1.5f, RigidType::Dynamic, 2.0f, glm::vec3(0.25f));

  auto block = add_entity<astralix::Object>("block", glm::vec3(0, 1.0f, -4.0f));

  block->get_component<TransformComponent>()->set_scale(glm::vec3(0.5f));

  block->add_component<MeshCollisionComponent>();
  // block->add_component<RigidBodyComponent>();

  ATTACH_MESH(block);
  ATTACH_LIGHTING_SHADER(block);
  // add_entity<Text>("text", "Text", glm::vec2(540.0f, 570.0f), 5.0f,
  //                  glm::vec3(0.3, 0.7f, 0.9f));
  // add_entity<PostProcessing>("Post Processing", "shaders::post_processing");
}

void Prologue::start() {
  load_resources();
  load_scene_components();
}

void Prologue::update() {}
