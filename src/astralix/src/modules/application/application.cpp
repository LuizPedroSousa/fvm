#include "application.hpp"
#include "ecs/managers/system-manager.hpp"
#include "engine.hpp"
#include "events/event-dispatcher.hpp"
#include "events/event-scheduler.hpp"
#include "glad/glad.h"
#include "time.hpp"
#include "trace.hpp"
#include "window.hpp"

namespace astralix {
Application *Application::m_instance = nullptr;

Application *Application::init() {
  m_instance = new Application;

  EventDispatcher::init();
  EventScheduler::init();
  Time::init();
  SystemManager::init();
  Engine::init();
  Window::init();

  return m_instance;
}

void Application::start() {
  Window::get()->open("FVM Editor", 1920, 1080);
  Engine::get()->start();
  SystemManager::get()->start();
}

void Application::run() {
  Window *window = Window::get();
  Engine *engine = Engine::get();
  auto system = SystemManager::get();
  Time *time = Time::get();

  auto scheduler = EventScheduler::get();

  while (window->is_open()) {
    time->update();
    window->update();
    scheduler->bind(SchedulerType::POST_FRAME);
    system->fixed_update(1 / 60.0f);
    system->update(Time::get()->get_deltatime());
    scheduler->bind(SchedulerType::IMMEDIATE);
    window->swap();
  }

  delete m_instance;
}

void Application::end() { delete m_instance; }

Application::~Application() {
  Engine::get()->end();
  Time::get()->end();
  Window::get()->close();
}

} // namespace astralix
