#pragma once
#include "functional"
#include "key-codes.hpp"
#include "listener.hpp"

namespace astralix {

class KeyEvent : public Event {

public:
  KeyCode get_key() { return m_key_code; }

protected:
  KeyEvent(const KeyCode key_code) : m_key_code(key_code){};
  KeyCode m_key_code;
};

class KeyPressedEvent : public KeyEvent {
public:
  KeyPressedEvent(const KeyCode key_code) : KeyEvent(key_code){};

  EVENT_CLASS_TYPE(KeyPressed)
};

class KeyReleasedEvent : public KeyEvent {
public:
  KeyReleasedEvent(const KeyCode key_code) : KeyEvent(key_code){};

  EVENT_CLASS_TYPE(KeyReleased)
};

class KeyboardListener : public BaseListener {
public:
  KeyboardListener(KeyEvent *event, const std::function<void()> &callback)
      : BaseListener(event), m_callback(callback) {}

  KeyEvent *get_event() override { return static_cast<KeyEvent *>(m_event); }

  void dispatch() override { m_callback(); }

  LISTENER_CLASS_TYPE(Keyboard)

private:
  std::function<void()> m_callback;
};

} // namespace astralix