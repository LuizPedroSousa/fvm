#include "mouse-event.hpp"
#include "iostream"

MouseDispatcher *MouseDispatcher::m_instance = nullptr;

MouseDispatcher *MouseDispatcher::get() {
  if (m_instance == nullptr) {
    m_instance = new MouseDispatcher;
  }

  return m_instance;
};

void MouseDispatcher::attach(std::function<void(MouseEvent event)> callback) {

  m_listeners.push_back({callback});
};

void MouseDispatcher::dispatch(MouseEvent event) {
  for (int i = 0; i < m_listeners.size(); i++) {
    m_listeners[i].callback(event);
  }
};