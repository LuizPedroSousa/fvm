#pragma once

#include "ecs/guid.hpp"
#include "events/event-scheduler.hpp"
#include "events/key-codes.hpp"
#include "log.hpp"
#include <unordered_map>

namespace astralix {

class Window;

class Keyboard {

public:
  static void init();
  static Keyboard *get() { return m_instance; };

  enum KeyEvent {
    KeyPressed = 0,
    KeyDown = 1,
    KeyReleased = 2,
  };

  struct KeyState {
    KeyEvent event;
    SchedulerID scheduler_id;
  };

  friend class Window;

  Keyboard();
  ~Keyboard();

  bool is_key_down(KeyCode key_code) {
    auto at = m_key_events.find(key_code);

    return at != nullptr && at->second.event == KeyEvent::KeyDown;
  }

  bool is_key_released(KeyCode key_code) {
    auto at = m_key_events.find(key_code);

    return at != nullptr && at->second.event == KeyEvent::KeyReleased;
  }

  void release_keys();

  void attach_key(KeyCode keycode, KeyState key_state) {
    m_key_events.emplace(keycode, key_state);
  };

  void destroy_key(KeyCode keycode) { m_key_events.erase(keycode); }

  void destroy_release_keys() {
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

private:
  std::unordered_map<KeyCode, KeyState> m_key_events;

  static Keyboard *m_instance;
};

#define IS_KEY_DOWN(keycode) Keyboard::get()->is_key_down(keycode)
#define IS_KEY_RELEASED(keycode) Keyboard::get()->is_key_released(keycode)

} // namespace astralix
