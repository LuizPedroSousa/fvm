#pragma once

#include "managers/component-manager.hpp"
#include "managers/entity-manager.hpp"
#include "managers/system-manager.hpp"
#include "resources/shader-renderer.hpp"
#include "resources/texture-renderer.hpp"
#include "scene.hpp"
#include "utils/guid.hpp"

class Prologue : public Entity<Prologue>, public Scene {
  public:
  Prologue(ENTITY_INIT_PARAMS);
  Prologue() {}

  Either<BaseException, Unit> start() override;
  void update() override;

  void on_enable(){};
  void on_disable(){};
};