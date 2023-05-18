#pragma once
#include "scene.hpp"
#include "memory"
#include "vector"

class Game
{
public:
  void start();
  void update();

  Game();

private:
  std::vector<std::unique_ptr<Scene>> m_scenes;
  int m_current_scene = 0;
};