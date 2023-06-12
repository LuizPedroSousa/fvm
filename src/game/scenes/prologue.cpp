#include "prologue.hpp"
#include "components/camera/camera-component.hpp"
#include "components/light/light-component.hpp"
#include "components/light/strategies/point-strategy.hpp"
#include "components/material/material-component.hpp"
#include "components/mesh-collision/mesh-collision-component.hpp"
#include "components/mesh/mesh-component.hpp"
#include "components/model/model-component.hpp"
#include "components/render/render-component.hpp"
#include "components/resource/resource-component.hpp"
#include "components/rigidbody/rigidbody-component.hpp"
#include "components/transform/transform-component.hpp"
#include "engine.hpp"
#include "glad/glad.h"
#include "managers/resource-manager.hpp"
#include "map"
#include "window.hpp"
#include <glm/gtx/string_cast.hpp>

Prologue::Prologue(ENTITY_INIT_PARAMS) : Scene(id, component_manager) {}

using astralix::BaseException;
using astralix::Either;
using astralix::Engine;
using astralix::Unit;

void Prologue::load_resources() {
  auto manager = Engine::get()->get_resource_manager();

  manager->load_texture("textures::window", "_texture", GL_RGBA,
                        "textures/window.png");

  manager->load_shaders({
      {"shaders::lighting", "vertex/light.glsl", "fragment/light.glsl"},
      {"shaders::outline", "vertex/light.glsl", "fragment/outline.glsl"},
      {"shaders::window", "vertex/light.glsl", "fragment/window.glsl"},
  });

  manager->load_models({
      {"models::player::warrior", "warrior/warrior.obj"},
      {"models::terrain::ground_top", "terrain/ground_grass_top.obj"},
      {"models::terrain::ground_middle", "terrain/ground_middle.obj"},
  });
}

void Prologue::load_scene_components() {
  auto camera = add_component<astralix::CameraComponent>(glm::vec3(0, 4.0f, 0));
  auto strategy = std::make_unique<astralix::PointStrategy>();
  auto light =
      add_component<astralix::LightComponent>(std::move(strategy), camera);

  light->add_source(glm::vec3(0.0f, 5.0f, 0.0f), "shaders::point_light_source");
}

Either<BaseException, Unit> Prologue::load_player() {

  auto create_player = []() {
    auto player =
        Engine::get()->get_entity_manager()->add_entity<astralix::Object>(
            glm::vec3(0.0f, 0.6f, 0.0f));

    player.get_component<astralix::ResourceComponent>()->attach_shader(
        "shaders::lighting");

    player.add_component<astralix::ModelComponent>()->attach_model(
        "models::player::warrior");
    player.add_component<astralix::MeshCollisionComponent>();

    player.get_component<astralix::RenderComponent>()->on_before_draw([]() {
      glStencilFunc(GL_ALWAYS, 1, 0xFF);
      glStencilMask(0xFF);
    });
  };

  auto create_outline = []() {
    auto player_outline =
        Engine::get()->get_entity_manager()->add_entity<astralix::Object>(
            glm::vec3(0.0f, 0.6f, 0.0f), glm::vec3(0.51f));
    player_outline.get_component<astralix::ResourceComponent>()->attach_shader(
        "shaders::outline");

    player_outline.add_component<astralix::ModelComponent>()->attach_model(
        "models::player::warrior");

    player_outline.get_component<astralix::RenderComponent>()->on_before_draw(
        []() {
          glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
          glStencilMask(0x00);
        });

    player_outline.get_component<astralix::RenderComponent>()->on_after_draw(
        []() {
          glStencilFunc(GL_ALWAYS, 1, 0xFF);
          glStencilMask(0xFF);
          glEnable(GL_DEPTH_TEST);
          glStencilMask(0xFF);
        });
  };
  ;

  create_player();
  create_outline();

  return Unit();
}

Either<BaseException, Unit> Prologue::load_terrain() {
  auto create_floor = [](u_int width, u_int height,
                         u_int distance_between_meshes) {
    auto object =
        Engine::get()->get_entity_manager()->add_entity<astralix::Object>(
            glm::vec3(0.0f, 0.0f, 0.0f));
    object.add_component<astralix::MeshCollisionComponent>();

    auto resource_component =
        object.get_component<astralix::ResourceComponent>();
    auto mesh_component = object.get_component<astralix::MeshComponent>();
    auto material_component =
        object.get_component<astralix::MaterialComponent>();

    auto models = Engine::get()->get_resource_manager()->get_models_by_ids(
        {"models::terrain::ground_top", "models::terrain::ground_middle"});

    std::vector<astralix::Mesh> terrain_meshes;

    auto top = models[0];
    auto middle = models[1];

    auto populate_top_meshes = [&](float x, float z) {
      for (int i = 0; i < top->meshes.size(); i++) {
        auto mesh = top->meshes[i];

        for (int j = 0; j < mesh.vertices.size(); j++) {
          mesh.vertices[j].position.z += z;
          mesh.vertices[j].position.x += x - 1.0f;
          mesh.vertices[j].texture_cordinates.x += x - 1.0f;
        }
        terrain_meshes.push_back(mesh);
      }
    };

    auto pupulate_middle_meshes = [&](float x, float z, float angle,
                                      glm::vec3 rotation) {
      for (int i = 0; i < middle->meshes.size(); i++) {
        auto mesh = middle->meshes[i];

        for (int j = 0; j < mesh.vertices.size(); j++) {

          glm::mat4 matrix_rotation(1.0f);

          matrix_rotation =
              glm::rotate(matrix_rotation, glm::radians(angle), rotation);

          glm::vec4 rotation =
              matrix_rotation * glm::vec4(mesh.vertices[j].position, 1.0f);

          mesh.vertices[j].position = rotation;

          mesh.vertices[j].position.z += z;
          mesh.vertices[j].position.x += x;
          mesh.vertices[j].position.y -= 1.0f;
          mesh.vertices[j].texture_cordinates.x += x;
        }
        terrain_meshes.push_back(mesh);
      }
    };

    auto is_heaf_of_max = [](float current_axis, float max_axis) -> bool {
      return current_axis > (max_axis / 2);
    };

    auto get_axis = [distance_between_meshes, is_heaf_of_max](
                        float current_axis, float max_axis) -> float {
      return distance_between_meshes * (is_heaf_of_max(current_axis, max_axis)
                                            ? (current_axis - max_axis)
                                            : current_axis);
    };

    for (int h = 0; h < height; h++) {
      float z = get_axis(h, height);

      for (int w = 0; w < width; w++) {
        float x = get_axis(w, width);

        if (h == height / 2) {
          pupulate_middle_meshes(x - 1.0f, z + 1.0f, 90.0f,
                                 glm::vec3(1.0f, 0.0f, 0.0f));
        } else if (z == -(height - 2)) {
          pupulate_middle_meshes(x - 1.0f, z - 1.0f, -90.0f,
                                 glm::vec3(1.0f, 0.0f, 0.0f));
        }

        if (w == width / 2) {
          pupulate_middle_meshes(x, z, -90.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        } else if (x == -(width - 2)) {
          pupulate_middle_meshes(x - 2.0f, z, 90.0f,
                                 glm::vec3(0.0f, 0.0f, 1.0f));
        }

        populate_top_meshes(x, z);
      }
    }

    resource_component->attach_shader("shaders::lighting");
    object.get_component<astralix::MaterialComponent>()->attach_materials(
        top->materials);
    object.get_component<astralix::MaterialComponent>()->attach_materials(
        middle->materials);
    object.get_component<astralix::MeshComponent>()->attach_meshes(
        terrain_meshes);

    object.get_component<astralix::RenderComponent>()->on_before_draw(
        []() { glStencilMask(0x00); });
  };

  auto create_cube = [&]() {
    auto cube =
        Engine::get()->get_entity_manager()->add_entity<astralix::Object>(
            glm::vec3(0.0f, 2.0f, 5.0f));
    auto cube_outline =
        Engine::get()->get_entity_manager()->add_entity<astralix::Object>(
            glm::vec3(0.0f, 2.0f, 5.0f), glm::vec3(0.51f));

    auto cube_mesh_component = cube.get_component<astralix::MeshComponent>();
    auto cube_outline_mesh_component =
        cube_outline.get_component<astralix::MeshComponent>();

    cube_mesh_component->attach_mesh(astralix::Mesh::cube_mesh(2.0f));
    cube_outline_mesh_component->attach_mesh(astralix::Mesh::cube_mesh(2.0f));

    cube.get_component<astralix::ResourceComponent>()->attach_shader(
        "shaders::lighting");
    cube_outline.get_component<astralix::ResourceComponent>()->attach_shader(
        "shaders::outline");

    cube.get_component<astralix::RenderComponent>()->on_before_draw(
        []() { glStencilMask(0xFF); });

    cube_outline.get_component<astralix::RenderComponent>()->on_before_draw(
        []() {
          glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
          glStencilMask(0x00);
          glDisable(GL_DEPTH_TEST);
        });

    cube_outline.get_component<astralix::RenderComponent>()->on_after_draw(
        []() {
          glStencilMask(0xFF);
          glStencilFunc(GL_ALWAYS, 1, 0xFF);
          glEnable(GL_DEPTH_TEST);
        });
  };

  auto create_window = [&](glm::vec3 position) {
    auto window =
        Engine::get()->get_entity_manager()->add_entity<astralix::Object>(
            position);
    auto resource = window.get_component<astralix::ResourceComponent>();
    auto render = window.get_component<astralix::RenderComponent>();

    render->on_before_draw([]() {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_CULL_FACE);
      glCullFace(GL_BACK);
    });

    render->on_after_draw([]() {
      glDisable(GL_BLEND);
      glDisable(GL_CULL_FACE);
    });

    resource->attach_shader("shaders::window");
    resource->attach_texture("textures::window");

    window.get_component<astralix::MeshComponent>()->attach_mesh(
        astralix::Mesh::cube_mesh(2.0f));
  };

  create_floor(24, 24, 1.0f);
  create_cube();

  std::map<float, glm::vec3> window_pos;

  auto camera = get_component<astralix::CameraComponent>();

  for (int i = 0; i < 4; i++) {
    auto pos = glm::vec3(0.0f, 1.0f, 4.0f * i);

    float distance = glm::length(camera->get_position() - pos);
    window_pos[distance] = pos;
  }

  for (std::map<float, glm::vec3>::reverse_iterator it = window_pos.rbegin();
       it != window_pos.rend(); ++it) {
    create_window(it->second);
  }

  return Unit();
}

Either<BaseException, Unit> Prologue::load_scene() {
  load_resources();
  load_scene_components();

  auto player = load_player();

  ASSERT_COMPARE(player);

  auto terrain = load_terrain();

  ASSERT_COMPARE(terrain);

  return Unit();
}

void Prologue::start() {
  auto scene_has_loaded = load_scene();

  ASSERT_COMPARE_THROW(scene_has_loaded);
}

void Prologue::update() {
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  glStencilFunc(GL_ALWAYS, 1, 0xFF);
}
