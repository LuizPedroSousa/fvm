#pragma once
#include "events/event.hpp"
#include "events/listener.hpp"
#include "websocket-server.hpp"

namespace astralix {

#define WIDTH 1920
#define HEIGHT 1080
#define PIXEL_SIZE 4 // RGBA

class ViewportEvent : public Event {
public:
  ViewportEvent() = default;

  EVENT_TYPE(Viewport)
};

class ViewportEventListener : public BaseListener {
public:
  ViewportEventListener() {}

  void dispatch(Event *event) override {
    std::vector<uint8_t> framebuffer(WIDTH * HEIGHT * PIXEL_SIZE);
    glReadPixels(0, 0, WIDTH, HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE,
                 framebuffer.data());

    auto app = WebsocketServer::get()->get_app();

    app->publish(
        "broadcast",
        std::string_view(reinterpret_cast<const char *>(framebuffer.data()),
                         framebuffer.size()),
        uWS::OpCode::BINARY, false);
  }

  LISTENER_CLASS_TYPE(ViewportListener)
};
} // namespace astralix
