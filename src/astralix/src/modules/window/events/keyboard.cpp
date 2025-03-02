#include "keyboard.hpp"

#include "events/event-scheduler.hpp"
#include "events/key-codes.hpp"
#include "log.hpp"
#include "window.hpp"

namespace astralix {

class Keyboard *Keyboard::m_instance = nullptr;

Keyboard::Keyboard() {}

void Keyboard::init() {
  if (m_instance == nullptr) {
    m_instance = new Keyboard;
  }
}

void Keyboard::release_key(KeyCode key) {
  LOG_INFO("KeyReleased");

  auto key_exists = m_key_events.find(key);

  if (key_exists == m_key_events.end())
    return;

  auto event = KeyReleasedEvent(key);

  EventDispatcher::get()->dispatch(&event);

  m_key_events[key].event = KeyEvent::KeyReleased;
}

void Keyboard::release_keys() {
  for (auto [key, value] : m_key_events) {

    auto state = glfwGetKey(Window::get_value(), key);

    LOG_INFO(" key ", key, " state ");

    if (state == GLFW_RELEASE) {
      LOG_INFO("KeyReleased");

      auto keycode = KeyReleasedEvent(KeyCode(key));

      EventDispatcher::get()->dispatch(&keycode);

      m_key_events[key].event = KeyEvent::KeyReleased;
    }
  }
}

void Keyboard::destroy_release_keys() {
  auto it = m_key_events.begin();
  auto scheduler = EventScheduler::get();

  for (; it != m_key_events.end();) {
    if (it->second.event == KeyEvent::KeyReleased) {
      if (scheduler->has_schedulers()) {
        scheduler->destroy(it->second.scheduler_id);
      }

      it = m_key_events.erase(it);
    } else {
      it++;
    }
  }
}

Keyboard::~Keyboard() { delete m_instance; }

} // namespace astralix
