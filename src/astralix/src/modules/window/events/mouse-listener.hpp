#pragma once
#include "listener.hpp"
#include "events/mouse-event.hpp"
#include "functional"

namespace astralix {

  class MouseListener : public BaseListener {
  public:
    MouseListener(const std::function<void(Event* event)>& callback)
      : m_callback(callback) {
    }

    void dispatch(Event* event) override { m_callback(event); }

    LISTENER_CLASS_TYPE(Mouse)

  private:
    std::function<void(Event* event)> m_callback;
  };

} // namespace astralix
