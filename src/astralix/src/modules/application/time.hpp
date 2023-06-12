#pragma once
#include "GLFW/glfw3.h"

class Time {
public:
  static void init();

  static Time *get() { return m_instance; }

  void end() { delete m_instance; }

  void update() {
    m_current_frame = glfwGetTime();
    m_deltatime = m_current_frame - m_last_frame;
    m_last_frame = m_current_frame;
  };

  float get_deltatime() { return m_deltatime; }
  float get_current_time() { return m_current_frame; }

private:
  float m_deltatime;
  float m_current_frame;
  float m_last_frame;

  static Time *m_instance;
};
