#pragma once
#include "event.hpp"
#include "guid.hpp"

namespace astralix {
enum ListenerType { Keyboard, Mouse, EntityListener, ViewportListener };

#define LISTENER_CLASS_TYPE(type)                                              \
  static ListenerType get_static_type() { return ListenerType::type; }         \
  virtual ListenerType get_listener_type() const override {                    \
    return get_static_type();                                                  \
  }                                                                            \
  virtual const char *get_name() const override { return #type; }

class BaseListener {
public:
  BaseListener() = default;
  virtual void dispatch(Event *event) = 0;

  virtual ListenerType get_listener_type() const = 0;
  virtual const char *get_name() const = 0;
  // virtual std::string to_string() const { return get_name(); }
  ListenerId get_id() { return m_id; };

private:
  ListenerId m_id;
};

} // namespace astralix
