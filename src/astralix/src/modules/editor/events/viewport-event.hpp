#pragma once
#include "ecs/managers/entity-manager.hpp"
#include "events/event.hpp"
#include "events/listener.hpp"
#include "managers/scene-manager.hpp"
#include "trace.hpp"
#include "websocket-server.hpp"
#include <cstdint>
#include <string>

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
  ViewportEventListener() : m_framebuffer(WIDTH * HEIGHT * PIXEL_SIZE) {}

  void dispatch(Event *event) override {
    ASTRA_PROFILE_N("Viewport listener");

    glReadPixels(0, 0, WIDTH, HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE,
                 m_framebuffer.data());

    auto socket = WebsocketServer::get();

    socket->publish("framebuffer", std::string(reinterpret_cast<const char *>(
                                                   m_framebuffer.data()),
                                               m_framebuffer.size()));

    auto scene = SceneManager::get()->get_active_scene()->serialize();

    Json::StreamWriterBuilder writer;

    std::string json_string = Json::writeString(writer, scene);

    socket->publish("scene", json_string);
  }

  LISTENER_CLASS_TYPE(ViewportListener)

private:
  std::vector<uint8_t> m_framebuffer;
};

} // namespace astralix
