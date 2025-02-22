#include "keyboard.hpp"

#include "events/event-scheduler.hpp"
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

Keyboard::~Keyboard() { delete m_instance; }

} // namespace astralix
