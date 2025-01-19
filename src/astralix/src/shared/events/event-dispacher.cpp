#include "event-dispatcher.hpp"

namespace astralix {

EventDispatcher *EventDispatcher::m_instance = nullptr;

void EventDispatcher::init() {
  if (m_instance == nullptr) {
    m_instance = new EventDispatcher;
  }
}

EventDispatcher *EventDispatcher::get() { return m_instance; }

} // namespace astralix