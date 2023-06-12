#include "time.hpp"

Time *Time::m_instance = nullptr;

void Time::init() {

  if (m_instance == nullptr) {
    m_instance = new Time;
  }
}