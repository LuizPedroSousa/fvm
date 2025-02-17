#pragma once

#include "entities/scene.hpp"

class Prologue : public astralix::Scene {
public:
  Prologue();

  void start() override;
  void update() override;

private:
  void load_scene_components();
  void load_resources();
};
