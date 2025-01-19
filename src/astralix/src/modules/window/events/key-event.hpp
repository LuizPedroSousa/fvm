#pragma once
#include "events/event.hpp"
#include "events/listener.hpp"
#include "functional"
#include "key-codes.hpp"

#define BASE_FIELDS KeyCode key_code;

namespace astralix {

class KeyPressedEvent : public Event {
public:
  KeyPressedEvent(KeyCode key_code) : key_code(key_code), Event() {}

  BASE_FIELDS
  EVENT_TYPE(KeyPressed)
};

class KeyReleasedEvent : public Event {
public:
  BASE_FIELDS;
  KeyReleasedEvent(KeyCode key_code) : key_code(key_code) {}
  EVENT_TYPE(KeyReleased)
};

class KeyboardListener : public BaseListener {
public:
  KeyboardListener(const std::function<void(Event *)> &callback)
      : m_callback(callback) {}

  void dispatch(Event *event) override { m_callback(event); }

  LISTENER_CLASS_TYPE(Keyboard)

private:
  std::function<void(Event *)> m_callback;
};
} // namespace astralix
