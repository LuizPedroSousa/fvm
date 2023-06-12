#pragma once

#include "ecs/guid.hpp"
#include "entities/scene.hpp"
#include "resources/shader-renderer.hpp"
#include "resources/texture-renderer.hpp"

class Prologue : public astralix::Scene {
public:
  Prologue(ENTITY_INIT_PARAMS);
  Prologue() {}

  void start() override;
  void update() override;

  void on_enable(){};
  void on_disable(){};

private:
  astralix::Either<astralix::BaseException, astralix::Unit> load_scene();

  void load_scene_components();
  astralix::Either<astralix::BaseException, astralix::Unit> load_player();
  astralix::Either<astralix::BaseException, astralix::Unit> load_terrain();
  void load_resources();
};