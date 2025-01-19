#include "event-scheduler.hpp"

namespace astralix {

EventScheduler *EventScheduler::m_instance = nullptr;

void EventScheduler::init() {
  if (m_instance == nullptr) {
    m_instance = new EventScheduler;
  }
}

EventScheduler *EventScheduler::get() { return m_instance; }

} // namespace astralix
