#include "prologue.hpp"
#include "components/camera/camera-component.hpp"
#include "components/light/light-component.hpp"
#include "components/light/strategies/point-strategy.hpp"
#include "components/material/material-component.hpp"
#include "components/mesh-collision/mesh-collision-component.hpp"
#include "components/mesh/mesh-component.hpp"
#include "components/model/model-component.hpp"
#include "components/post-processing/post-processing-component.hpp"
#include "components/resource/resource-component.hpp"
#include "components/rigidbody/rigidbody-component.hpp"
#include "components/skybox/skybox-component.hpp"
#include "components/transform/transform-component.hpp"
#include "engine.hpp"
#include "glad/glad.h"
#include "managers/resource-manager.hpp"
#include "map"
#include "window.hpp"
#include <glm/gtx/string_cast.hpp>

Prologue::Prologue() : Scene() {}

using astralix::Engine;

void Prologue::load_resources() {
  auto manager = astralix::ResourceManager::get();

  manager->load_textures(
      {astralix::Texture2D::create("textures::default", "textures/diffuse.png",
                                   false),
       astralix::Texture3D::create("cubemaps::skybox",
                                   {
                                       "textures/skybox/right.jpg",
                                       "textures/skybox/left.jpg",
                                       "textures/skybox/top.jpg",
                                       "textures/skybox/bottom.jpg",
                                       "textures/skybox/front.jpg",
                                       "textures/skybox/back.jpg",
                                   })});

  manager->load_shaders(
      {astralix::Shader::create(
           "shaders::visualize_normal", "fragment/normal_view.glsl",
           "vertex/normal_view.glsl", "geometry/normal_view.glsl"),
       astralix::Shader::create("shaders::lighting", "fragment/light.glsl",
                                "vertex/light.glsl"),
       astralix::Shader::create("shaders::skybox", "fragment/skybox.glsl",
                                "vertex/skybox.glsl")});
}

void Prologue::load_scene_components() {
  auto manager = astralix::EntityManager::get();

  auto camera = manager->add_entity<astralix::Object>("camera");
  camera.add_component<astralix::CameraComponent>();

  auto strategy = astralix::create_scope<astralix::PointStrategy>();

  auto light = manager->add_entity<astralix::Object>("Light");

  light.add_component<astralix::LightComponent>(std::move(strategy),
                                                camera.get_entity_id());

  manager->add_entity<astralix::Object>("Skybox")
      .add_component<astralix::SkyboxComponent>("cubemaps::skybox",
                                                "shaders::skybox");
}

void Prologue::start() {
  load_resources();
  load_scene_components();
}

void Prologue::update() {}
