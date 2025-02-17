#include "prologue.hpp"
#include "components/camera/camera-component.hpp"
#include "components/light/light-component.hpp"
#include "components/light/strategies/directional-strategy.hpp"
#include "components/light/strategies/point-strategy.hpp"
#include "components/material/material-component.hpp"
#include "components/mesh-collision/mesh-collision-component.hpp"
#include "components/mesh/mesh-component.hpp"
#include "components/model/model-component.hpp"
#include "components/post-processing/post-processing-component.hpp"
#include "components/resource/resource-component.hpp"
#include "components/rigidbody/rigidbody-component.hpp"
#include "components/transform/transform-component.hpp"
#include "engine.hpp"
#include "entities/object.hpp"
#include "entities/post-processing.hpp"
#include "entities/skybox.hpp"
#include "glad/glad.h"
#include "glm/fwd.hpp"
#include "managers/resource-manager.hpp"
#include "map"
#include "project.hpp"
#include "resources/model.hpp"
#include "window.hpp"
#include <filesystem>
#include <glm/gtx/string_cast.hpp>

Prologue::Prologue() : Scene("prologue") {}

using namespace astralix;

using astralix::Engine;

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
      {Texture2D::create("textures::default", "textures/diffuse.png", false),
       Texture3D::create("cubemaps::skybox", {
                                                 "textures/skybox/right.jpg",
                                                 "textures/skybox/left.jpg",
                                                 "textures/skybox/top.jpg",
                                                 "textures/skybox/bottom.jpg",
                                                 "textures/skybox/front.jpg",
                                                 "textures/skybox/back.jpg",
                                             })});

  manager->load_models(
      {Model::create("module::grass::flat", "models/Grass_Flat.obj")});
  manager->load_shaders(
      {Shader::create("shaders::post_processing",
                      "fragment/postprocessing.glsl",
                      "vertex/postprocessing.glsl"),
       Shader::create("shaders::lighting", "fragment/light.glsl",
                      "vertex/light.glsl"),
       Shader::create("shaders::skybox", "fragment/skybox.glsl",
                      "vertex/skybox.glsl")});
}

void create_tile_grid(Ref<EntityManager> manager, int columns, int rows,

                      float tile_size,

                      RigidType type = RigidType::Static, float y = 0.0f,
                      glm::vec3 scale = glm::vec3(1.0)) {
  float offset_x = (columns * tile_size) / 2.0f - tile_size / 2.0f;
  float offset_z = (rows * tile_size) / 2.0f - tile_size / 2.0f;

  for (int col = 0; col < columns; ++col) {
    for (int row = 0; row < rows; ++row) {
      float x = col * tile_size - offset_x;
      float z = row * tile_size - offset_z;

      auto tile = manager->add_entity<Object>("tile", glm::vec3(x, y, z));

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
  auto manager = astralix::EntityManager::get();

  auto camera = manager->add_entity<astralix::Object>(
      "camera", glm::vec3(0.0f, 0.0f, 3.0f));

  camera->add_component<CameraComponent>();

  auto strategy = astralix::create_scope<astralix::DirectionalStrategy>();

  auto light = manager->add_entity<astralix::Object>(
      "Light", glm::vec3(-2.0f, 4.0f, -1.0f));

  light->add_component<LightComponent>(std::move(strategy),
                                       camera->get_entity_id());

  manager->add_entity<Skybox>("skybox", "cubemaps::skybox", "shaders::skybox");

  create_tile_grid(manager, 6, 6, 1.0f, RigidType::Static);
  create_tile_grid(manager, 4, 4, 1.5f, RigidType::Dynamic, 2.0f,
                   glm::vec3(0.25f));

  auto block = manager->add_entity<astralix::Object>("ground",
                                                     glm::vec3(0, 10.0f, 0.0f));

  block->get_component<TransformComponent>()->set_scale(glm::vec3(0.5f));

  block->add_component<MeshCollisionComponent>();
  block->add_component<RigidBodyComponent>();

  ATTACH_MESH(block);
  ATTACH_LIGHTING_SHADER(block);

  // manager->add_entity<PostProcessing>("Post Processing",
  //                                     "shaders::post_processing");
}

void Prologue::start() {
  load_resources();
  load_scene_components();
}

void Prologue::update() {}
