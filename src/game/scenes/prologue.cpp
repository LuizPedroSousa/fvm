#include "prologue.hpp"
#include "components/camera/camera-component.hpp"
#include "components/light/light-component.hpp"
#include "components/light/strategies/directional-strategy.hpp"
#include "components/mesh-collision/mesh-collision-component.hpp"
#include "components/mesh/mesh-component.hpp"
#include "components/model/model-component.hpp"
#include "components/resource/resource-component.hpp"
#include "components/rigidbody/rigidbody-component.hpp"
#include "components/transform/transform-component.hpp"
#include "entities/object.hpp"
#include "entities/post-processing.hpp"
#include "entities/skybox.hpp"
#include "entities/text.hpp"
#include "glad/glad.h"
#include "glm/fwd.hpp"
#include "managers/resource-manager.hpp"
#include "resources/model.hpp"
#include <glm/gtx/string_cast.hpp>

Prologue::Prologue() : Scene("prologue") {}

#define ATTACH_MESH(var)                                                       \
  var->add_component<MeshComponent>()->attach_mesh(astralix::Mesh::cube(1.0f));
#define ATTACH_LIGHTING_SHADER(var)                                            \
  var->get_component<ResourceComponent>()                                      \
      ->attach_shader("shaders::lighting")                                     \
      ->attach_texture({"textures::default", "materials[0].diffuse"})          \
      ->attach_texture({"textures::default", "materials[0].specular"})

void Prologue::load_resources() {
  auto manager = ResourceManager::get();

  manager->load_textures(
      ///
      {
          Texture2D::create("textures::default", "textures/diffuse.png", false),
          Texture3D::create("cubemaps::skybox",
                            {
                                "textures/skybox/right.jpg",
                                "textures/skybox/left.jpg",
                                "textures/skybox/top.jpg",
                                "textures/skybox/bottom.jpg",
                                "textures/skybox/front.jpg",
                                "textures/skybox/back.jpg",
                            })
          ///
      });

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

  manager->load_fonts(
      ///
      {
          Font::create("roboto", "fonts/roboto.ttf"),
          ///
      }

  );
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

      tile->get_component<ResourceComponent>()->attach_shader(
          "shaders::lighting");

      // tile->get_component<TransformComponent>()->set_scale(glm::vec3(0.5f));
      // ATTACH_MESH(tile);
      tile->add_component<ModelComponent>()->attach_model(
          "module::grass::flat");

      tile->get_component<TransformComponent>()->set_scale(scale);

      tile->add_component<MeshCollisionComponent>();
      tile->add_component<RigidBodyComponent>(type);
    }
  }
}
void Prologue::load_scene_components() {

  auto camera =
      add_entity<astralix::Object>("camera", glm::vec3(0.0f, 0.0f, 3.0f));

  camera->add_component<CameraComponent>();

  auto strategy = astralix::create_scope<astralix::DirectionalStrategy>();

  auto light =
      add_entity<astralix::Object>("Light", glm::vec3(-2.0f, 4.0f, -1.0f));

  light->add_component<LightComponent>(std::move(strategy),
                                       camera->get_entity_id());

  add_entity<Skybox>("skybox", "cubemaps::skybox", "shaders::skybox");

  create_tile_grid(6, 6, 1.0f, RigidType::Static);
  create_tile_grid(4, 4, 1.5f, RigidType::Dynamic, 2.0f, glm::vec3(0.25f));

  auto block =
      add_entity<astralix::Object>("ground", glm::vec3(0, 10.0f, 0.0f));

  block->get_component<TransformComponent>()->set_scale(glm::vec3(0.5f));

  block->add_component<MeshCollisionComponent>();
  block->add_component<RigidBodyComponent>();

  ATTACH_MESH(block);
  ATTACH_LIGHTING_SHADER(block);
  add_entity<Text>("text", "Text", glm::vec2(540.0f, 570.0f), 5.0f,
                   glm::vec3(0.3, 0.7f, 0.9f));
  // add_entity<PostProcessing>("Post Processing", "shaders::post_processing");
}

void Prologue::start() {
  load_resources();
  load_scene_components();
}

void Prologue::update() {}
