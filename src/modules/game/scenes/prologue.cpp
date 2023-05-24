#include "prologue.hpp"
#include "components/camera-component.hpp"
#include "components/light-component.hpp"
#include "components/material-component.hpp"
#include "components/resource-component.hpp"
#include "components/transform-component.hpp"
#include "game.hpp"
#include <glm/gtx/string_cast.hpp>

#include "glad/glad.h"

Prologue::Prologue(ENTITY_INIT_PARAMS) : Entity(id, component_manager) {
}

Either<BaseException, Unit> Prologue::start() {

  auto object = Game::get()->get_entity_manager()->add_entity<Object>(Object::cube(), glm::vec3(0.0f, 0.0f, 0.0));

  // load shaders
  auto resource_component = object.get_component<ResourceComponent>();
  auto shader_loaded = resource_component->load_shader(Shader::create("vertex/triangle.glsl", "fragment/triangle.glsl"));
  ASSERT_COMPARE(shader_loaded);
  // --------------

  // load maps
  auto material = object.add_component<MaterialComponent>();
  auto maps_loaded = material->load_maps(
      Texture::create("material.diffuse", "box.png", GL_RGBA),
      Texture::create("material.specular", "box-specular.png", GL_RGBA),
      0.6f);
  ASSERT_COMPARE(maps_loaded);
  // --------------

  auto camera = add_component<CameraComponent>(glm::vec3(0, 0, 3.0f));

  auto light_object = Game::get()->get_entity_manager()->add_entity<Object>(
      Object::cube(), glm::vec3(1.2f, 1.0f, 2.0f));

  light_object.add_component<LightComponent>(camera);

  return Unit();
}

void Prologue::update() {
}