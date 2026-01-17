#include "mouse.hpp"
#include "window.hpp"

namespace astralix {

class Mouse *Mouse::m_instance = nullptr;

Mouse::Mouse() {
  m_last.x = WINDOW_HEIGHT() / 2;
  m_last.y = WINDOW_WIDTH() / 2;
}

void Mouse::init() {
  if (m_instance == nullptr) {
    m_instance = new Mouse;
  }
}

Mouse::~Mouse() { delete m_instance; }

} // namespace astralix
