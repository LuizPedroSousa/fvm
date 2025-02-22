#pragma once

#include "base.hpp"
#include "events/event-scheduler.hpp"
#include "events/key-codes.hpp"
#include "events/key-event.hpp"
#include "events/mouse-event.hpp"
#include "uwebsockets/App.h"
#include "window.hpp"
#include "json/json.h"
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <json/reader.h>
#include <memory>
#include <string>
#include <thread>
#include <vector>

namespace astralix {
class WebsocketServer {
public:
  WebsocketServer();

  static void init();

  struct PerSocketData {
    /* Fill with user data */
  };

  void start() {
    m_app = new uWS::SSLApp(
        {/* There are example certificates in uWebSockets.js repo */
         .key_file_name = "misc/key.pem",
         .cert_file_name = "misc/cert.pem",
         .passphrase = "1234"});

    m_app
        ->ws<PerSocketData>(
            "/*",
            {/* Settings */
             .compression = uWS::SHARED_COMPRESSOR,
             .maxPayloadLength = 16 * 1024 * 1024,
             .idleTimeout = 16,
             .maxBackpressure = 1 * 1024 * 1024,
             .closeOnBackpressureLimit = false,
             .resetIdleTimeoutOnSend = false,
             .sendPingsAutomatically = true,
             /* Handlers */
             .upgrade = nullptr,
             .open =
                 [](auto *ws) {
                   /* Open event here, you may access ws->getUserData() which
                    * points to a PerSocketData struct */
                   ws->subscribe("broadcast");
                 },
             .message =
                 [](auto * /*ws*/, std::string_view message,
                    uWS::OpCode /*opCode*/) {
                   Json::Value root;

                   Json::CharReaderBuilder rbuilder;

                   rbuilder["collectComments"] = false;

                   std::stringstream ss;
                   ss.write(message.data(), message.size());

                   std::string errs;
                   bool ok = Json::parseFromStream(rbuilder, ss, &root, &errs);

                   auto type = root["type"].asString();
                   auto action = root["action"].asString();

                   auto scheduler = EventScheduler::get();

                   if (type == "keyboard") {
                     auto keyCode = root["keyCode"].as<int>();

                     if (action == "press") {
                       std::cout << "KeyPressed " << keyCode << "\n";
                       auto event = KeyCode(keyCode);

                       auto scheduler_id = scheduler->schedule<KeyPressedEvent>(
                           SchedulerType::POST_FRAME, event);

                       //                      Window::get()->attach_key(keyCode,
                       //                      scheduler_id);
                       return;
                     }

                     std::cout << "KeyReleased " << keyCode << "\n";
                     auto keycode = KeyReleasedEvent(KeyCode(keyCode));

                     auto window = Window::get();
                     // auto scheduler_id =
                     // window->get_key_scheduler_id(keyCode);
                     // scheduler->destroy(scheduler_id);
                     // window->destroy_key(keyCode);

                     EventDispatcher::get()->dispatch(&keycode);
                     return;
                   }

                   auto x = root["x"].as<int>();
                   auto y = root["y"].as<int>();

                   std::cout << "MouseEvent x " << x << "\n";
                   std::cout << "MouseEvent y " << y << "\n";

                   EventDispatcher::get()->dispatch(new MouseEvent(x, y));
                 },
             .drain =
                 [](auto * /*ws*/) {
                   /* Check ws->getBufferedAmount() here */
                 },
             .ping =
                 [](auto * /*ws*/, std::string_view) {
                   /* Not implemented yet */
                 },
             .pong =
                 [](auto * /*ws*/, std::string_view) {
                   /* Not implemented yet */
                 },
             .close =
                 [](auto * /*ws*/, int /*code*/, std::string_view /*message*/) {
                   /* You may access ws->getUserData() here */
                 }})
        .listen(9001, [](auto *listen_socket) {
          if (listen_socket) {
            std::cout << "Listening on port " << 9001 << std::endl;
          }
        });

    if (!m_app) {
      std::cerr << "SSLApp is not initialized!" << std::endl;
    }
  }

  static WebsocketServer *get() { return m_instance; }

  void publish(std::string_view topic, std::string_view message,
               uWS::OpCode opCode, bool compress = false) {
    m_app->publish(topic, message, opCode, compress);
  }

  uWS::SSLApp *get_app() { return m_app; }

  struct us_timer_t *get_timer() {
    struct us_loop_t *loop = (struct us_loop_t *)uWS::Loop::get();
    struct us_timer_t *delayTimer = us_create_timer(loop, 0, 0);

    return delayTimer;
  };

  void update() { m_app->run(); }

  ~WebsocketServer() { delete m_app; }

private:
  int m_port;
  int m_threads_length;

  uWS::SSLApp *m_app = nullptr;

  static WebsocketServer *m_instance;
  std::vector<std::thread> m_thread_pool;
};
} // namespace astralix
