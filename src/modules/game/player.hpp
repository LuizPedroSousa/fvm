#pragma once
#include "camera.hpp"
#include "object.hpp"
#include "object-renderer.hpp"
#include "shader-renderer.hpp"

class Player
{
private:
  ObjectRenderer m_o_renderer;
  ShaderRenderer m_s_renderer;

public:
  Player(unsigned int render_id);
  Camera m_camera;

  void render();
  void start();
};