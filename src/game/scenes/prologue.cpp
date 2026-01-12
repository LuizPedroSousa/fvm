#include "prologue.hpp"

#include "base.hpp"
#include "components/light/light-component.hpp"
#include "components/light/strategies/directional-strategy.hpp"
#include "components/light/strategies/point-strategy.hpp"
#include "components/material/material-component.hpp"
#include "components/mesh-collision/mesh-collision-component.hpp"
#include "components/mesh/mesh-component.hpp"
#include "components/resource/resource-component.hpp"
#include "components/rigidbody/rigidbody-component.hpp"
#include "components/transform/transform-component.hpp"
#include "entities/camera.hpp"
#include "entities/object.hpp"
#include "entities/serializers/scene-serializer.hpp"
#include "entities/skybox.hpp"
#include "entities/text.hpp"
#include "events/key-codes.hpp"
#include "events/keyboard.hpp"
#include "glad/glad.h"
#include "glm/fwd.hpp"
#include "log.hpp"
#include "managers/resource-manager.hpp"
#include "omp.h"
#include "resources/material.hpp"
#include "gtest/gtest.h"
#include <glm/gtx/string_cast.hpp>

Prologue::Prologue() : Scene("prologue") {}

#define ATTACH_MESH(var)                                                       \
  var->add_component<MeshComponent>()->attach_mesh(astralix::Mesh::cube(1.0f));
#define ATTACH_LIGHTING_SHADER(var)                                            \
  var->get_component<ResourceComponent>()->set_shader("shaders::lighting");    \
  var->get_or_add_component<MaterialComponent>()->attach_material(             \
      "materials::wood")

void Prologue::load_resources() {
  auto manager = ResourceManager::get();

  manager->load_textures(
      {Texture3D::create("cubemaps::skybox",
                         {
                             "textures/skybox/right.jpg",
                             "textures/skybox/left.jpg",
                             "textures/skybox/top.jpg",
                             "textures/skybox/bottom.jpg",
                             "textures/skybox/front.jpg",
                             "textures/skybox/back.jpg",
                         }),

       Texture2D::create(
           "textures::wood::diffuse", "textures/wood.png", false,

           {//
            {TextureParameter::WrapS, TextureValue::Linear},
            {TextureParameter::WrapT, TextureValue::Linear},

            {TextureParameter::MagFilter, TextureValue::LinearMipMap},
            {TextureParameter::MinFilter, TextureValue::Linear}

           }

           //
           ),

       Texture2D::create(
           "textures::wood::normal", "textures/wood_normal.png", false,

           {//
            {TextureParameter::WrapS, TextureValue::Linear},
            {TextureParameter::WrapT, TextureValue::Linear},
            {TextureParameter::MagFilter, TextureValue::LinearMipMap},
            {TextureParameter::MinFilter, TextureValue::Linear}

           }

           ),
       Texture2D::create(
           "textures::wood::displacement", "textures/wood_displacement.png",
           false,

           {//
            {TextureParameter::WrapS, TextureValue::Linear},
            {TextureParameter::WrapT, TextureValue::Linear},
            {TextureParameter::MagFilter, TextureValue::LinearMipMap},
            {TextureParameter::MinFilter, TextureValue::Linear}

           })

      });

  manager->load_materials({Material::create(
      "materials::wood", {"textures::wood::diffuse"}, {},
      "textures::wood::normal", "textures::wood::displacement")});

  manager->load_shaders(
      ///
      {Shader::create("shaders::post_processing ",
                      "fragment/postprocessing.glsl",
                      "vertex/postprocessing.glsl"),
       Shader::create("shaders::lighting", "fragment/light.glsl",
                      "vertex/light.glsl"),
       Shader::create("shaders::skybox", "fragment/skybox.glsl",
                      "vertex/skybox.glsl"),
       Shader::create("shaders::glyph", "fragment/glyph.glsl",
                      "vertex/glyph.glsl")

      }

  );
}

static std::vector<Tile> tiles;

void Prologue::create_tile_grid(int columns, int rows, float tile_size,
                                RigidType type, float y, glm::vec3 scale) {
  float offset_x = (columns - 1) * tile_size * 0.5f;
  float offset_z = (rows - 1) * tile_size * 0.5f;

  tiles.resize(columns * rows);

  std::vector<glm::vec3> positions;

  positions.resize(columns * rows);

#pragma omp parallel for collapse(2)
  for (int col = 0; col < columns; ++col) {
    for (int row = 0; row < rows; ++row) {
      float x = col * tile_size - offset_x;
      float z = row * tile_size - offset_z;

      int index = col * rows + row;

      positions[index] = glm::vec3(x, y, z);
    }
  }

  auto size = positions.size();

  for (int i = 0; i < size; ++i) {
    auto &position = positions[i];

    auto tile = add_entity<Object>("tile", position);

    auto meshComp = tile->add_component<MeshComponent>();
    meshComp->attach_mesh(Mesh::cube(1.0f));

    ATTACH_LIGHTING_SHADER(tile);

    auto transform = tile->get_component<TransformComponent>();
    transform->set_scale(scale);

    tile->add_component<MeshCollisionComponent>();
    tile->add_component<RigidBodyComponent>(type);

    tiles[i] = Tile{tile, position};

    LOG_DEBUG(i);
  }
}

void Prologue::load_scene_components() {
  auto camera = add_entity<Camera>("camera", CameraMode::Free,
                                   glm::vec3(0.0f, 4.0f, 0.0f));

  auto directional_light =
      add_entity<astralix::Object>("Light", glm::vec3(-2.0f, 4.0f, -1.0f));

  directional_light->add_component<LightComponent>(camera->get_entity_id())
      ->strategy(create_scope<DirectionalStrategy>());

  std::vector<glm::vec3> lightColors;
  lightColors.push_back(glm::vec3(0.5f, 0.0f, 0.0f));
  lightColors.push_back(glm::vec3(0.0f, 0.0f, 0.5f));
  lightColors.push_back(glm::vec3(0.0f, 0.5f, 0.0f));

  for (int i = 0; i < lightColors.size(); i++) {
    auto point_light = add_entity<astralix::Object>(
        "Light", glm::vec3(i * 2.0f - 0.5f, 2.0f, i * 2.0f));

    point_light->get_component<ResourceComponent>()->set_shader(
        "shaders::lighting");

    point_light->add_component<MeshComponent>()->attach_mesh(Mesh::cube(1.0f));

    // point_light->add_component<LightComponent>(camera->get_entity_id())
    //     ->strategy(create_scope<PointStrategy>(PointStrategy::Point{
    //         .exposure = PointStrategy::Exposure{.ambient = lightColors[i],
    //                                             .diffuse = lightColors[i],
    //                                             .specular =
    //                                             lightColors[i]}}));
  }

  add_entity<Skybox>("skybox", "cubemaps::skybox", "shaders::skybox");

  create_tile_grid(16, 16, 1.0f, RigidType::Static);
  create_tile_grid(12, 12, 0.025f, RigidType::Dynamic, 20.0f,
                   glm::vec3(0.025f));
}

void Prologue::start() {
  load_resources();
  load_scene_components();
}

void Prologue::update() {

  if (IS_KEY_DOWN(KeyCode::F5)) {
    for (auto tile : tiles) {
      auto transform = tile.object->get_component<TransformComponent>();

      transform->translate(tile.position);
    }
  }
}
