#pragma once

#include "components/rigidbody/rigidbody-component.hpp"
#include "entities/object.hpp"
#include "entities/scene.hpp"

using namespace astralix;

class Prologue : public Scene {
public:
  Prologue();

  void start() override;
  void update() override;

private:
  void create_tile_grid(int columns, int rows,

                        float tile_size,

                        RigidType type = RigidType::Static, float y = 0.0f,
                        glm::vec3 scale = glm::vec3(1.0));

  void load_scene_components();
  void load_resources();
};
