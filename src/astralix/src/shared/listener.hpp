#pragma once
#include "string"

namespace astralix {

enum EventType { KeyPressed = 0, KeyReleased = 1, MouseMovement };
enum ListenerType { Keyboard, Mouse };

#define EVENT_CLASS_TYPE(type)                                                 \
  static EventType get_static_type() { return EventType::type; }               \
  virtual EventType get_event_type() const override {                          \
    return get_static_type();                                                  \
  }                                                                            \
  virtual const char *get_name() const override { return #type; }

#define LISTENER_CLASS_TYPE(type)                                              \
  static ListenerType get_static_type() { return ListenerType::type; }         \
  virtual ListenerType get_listener_type() const override {                    \
    return get_static_type();                                                  \
  }                                                                            \
  virtual const char *get_name() const override { return #type; }

class Event {
public:
  virtual EventType get_event_type() const = 0;

protected:
  virtual const char *get_name() const = 0;
  virtual std::string to_string() const { return get_name(); }
};

class BaseListener {
public:
  BaseListener(Event *event) : m_event(event) {}
  virtual ~BaseListener() {}
  virtual void dispatch() = 0;

  virtual ListenerType get_listener_type() const = 0;
  virtual const char *get_name() const = 0;
  virtual std::string to_string() const { return get_name(); }

  virtual Event *get_event() { return m_event; }

  void set_event(Event *event) {
    delete m_event;
    m_event = nullptr;
    m_event = event;
  }

protected:
  Event *m_event;
};

} // namespace astralix
