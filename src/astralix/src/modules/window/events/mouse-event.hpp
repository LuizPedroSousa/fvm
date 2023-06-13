#pragma once
#include "functional"
#include "listener.hpp"

namespace astralix {

class MouseEvent : public Event {
public:
  MouseEvent(double x, double y) : m_x(x), m_y(y){};

  MouseEvent(){};

  double get_x() { return m_x; }
  double get_y() { return m_y; }

  EVENT_CLASS_TYPE(MouseMovement)

private:
  double m_x;
  double m_y;
};

class MouseListener : public BaseListener {
public:
  MouseListener(MouseEvent *event,
                const std::function<void(MouseEvent *event)> &callback)
      : BaseListener(event), m_callback(callback) {}

  void dispatch() override { m_callback((MouseEvent *)m_event); }

  MouseEvent *get_event() override {
    return static_cast<MouseEvent *>(m_event);
  }

  LISTENER_CLASS_TYPE(Mouse)

private:
  std::function<void(MouseEvent *event)> m_callback;
};

} // namespace astralix