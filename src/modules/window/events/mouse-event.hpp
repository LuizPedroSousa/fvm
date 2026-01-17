#pragma once
#include "events/event.hpp"

namespace astralix {
struct MouseEvent : public Event {
public:
  MouseEvent(double x, double y) : x(x), y(y) {}
  double x;
  double y;
  EVENT_TYPE(MouseMovement)
};
} // namespace astralix
