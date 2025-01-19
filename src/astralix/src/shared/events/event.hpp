#pragma once
namespace astralix {
enum EventType {
  /* KEYBOARD */
  KeyPressed = 0,
  KeyReleased = 1,
  /* MOUSE */
  MouseMovement = 2,

  /* ENTITY */
  EntityCreated = 3,
  Viewport = 4
};

class Event {
public:
  Event() = default;
  virtual EventType get_event_type() const = 0;
};

#define EVENT_TYPE(t)                                                          \
  static EventType get_static_type() { return EventType::t; }                  \
  EventType get_event_type() const override { return get_static_type(); }

} // namespace astralix
