#pragma once
#include "log.hpp"

#include "ecs/guid.hpp"
#include "events/key-codes.hpp"
#include <unordered_map>

namespace astralix {

class Mouse {
public:
  struct Position {
    double x;
    double y;

    void operator*=(float factor) {
      x *= factor;
      y *= factor;
    }
  };

  static void init();
  static Mouse *get() { return m_instance; };

  Position delta() {
    return Position{
        m_current.x,
        m_current.y,
    };
  };

  void set_position(Position position) {
    if (m_is_first_recalculation) {
      m_last = position;
      m_is_first_recalculation = false;
    }

    double dx = position.x - m_last.x;
    double dy = position.y - m_last.y;

    m_current.x += dx;
    m_current.y += dy;

    m_last = position;

    m_changed = true;
  }

  void reset_delta() {
    m_current = {.x = 0, .y = 0};
    m_changed = false;
  }

  bool has_moved() { return m_changed; }

  friend class Window;

  Mouse();
  ~Mouse();

private:
  Position m_current;
  Position m_last;

  bool m_is_first_recalculation = true;
  bool m_changed = false;

  static Mouse *m_instance;
};

#define MOUSE_DELTA() Mouse::get()->delta()

#define HAS_MOUSE_MOVED() Mouse::get()->has_moved()

#define SET_MOUSE_POSITION(position) Mouse::get()->set_position(position)

} // namespace astralix
