#include "glad/glad.h"

#include "application.hpp"
#include "time.hpp"
#include "window.hpp"

namespace astralix {
Application *Application::m_instance = nullptr;

Application *Application::init() {
  m_instance = new Application;

  Window::init();
  Engine::init();
  Time::init();

  return m_instance;
}

void Application::start() {
  Window::get()->open(800, 600);
  Engine::get()->start();
}

void Application::run() {
  Window *window = Window::get();
  Engine *engine = Engine::get();
  Time *time = Time::get();

  while (window->is_open()) {
    time->update();

    window->update();
    engine->update();
    window->post_update();
  }

  delete m_instance;
}

Application::~Application() {
  Engine::get()->end();
  Time::get()->end();
  Window::get()->close();
}

} // namespace astralix
