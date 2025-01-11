#pragma once

#include "base.hpp"
#include "uwebsockets/App.h"
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "time.hpp"
#include "window.hpp"
#include "editor.hpp"
#include "engine.hpp"

namespace astralix
{



  class WebsocketServer {
  public:
    WebsocketServer();

    static void init();

    struct PerSocketData {
      /* Fill with user data */
    };


    void start() {
      uWS::SSLApp app = uWS::SSLApp({
        /* There are example certificates in uWebSockets.js repo */
      .key_file_name = "misc/key.pem",
      .cert_file_name = "misc/cert.pem",
      .passphrase = "1234"
        }).ws<PerSocketData>("/*", {
          /* Settings */
          .compression = uWS::SHARED_COMPRESSOR,
          .maxPayloadLength = 16 * 1024 * 1024,
          .idleTimeout = 16,
          .maxBackpressure = 1 * 1024 * 1024,
          .closeOnBackpressureLimit = false,
          .resetIdleTimeoutOnSend = false,
          .sendPingsAutomatically = true,
          /* Handlers */
          .upgrade = nullptr,
          .open = [](auto* ws) {
            /* Open event here, you may access ws->getUserData() which points to a PerSocketData struct */
            ws->subscribe("broadcast");
        },
        .message = [](auto*/*ws*/, std::string_view /*message*/, uWS::OpCode /*opCode*/) {

        },
        .drain = [](auto*/*ws*/) {
            /* Check ws->getBufferedAmount() here */
        },
        .ping = [](auto*/*ws*/, std::string_view) {
            /* Not implemented yet */
        },
        .pong = [](auto*/*ws*/, std::string_view) {
            /* Not implemented yet */
        },
        .close = [](auto*/*ws*/, int /*code*/, std::string_view /*message*/) {
            /* You may access ws->getUserData() here */
        }
          }).listen(9001, [](auto* listen_socket) {
            if (listen_socket) {
              std::cout << "Listening on port " << 9001 << std::endl;
            }
            });



      m_app = &app;

      struct us_loop_t* loop = (struct us_loop_t*)uWS::Loop::get();
      struct us_timer_t* delayTimer = us_create_timer(loop, 0, 0);

      us_timer_set(delayTimer, [](struct us_timer_t*/*t*/) {
        Window* window = Window::get();
        Engine* engine = Engine::get();
        auto system = SystemManager::get();
        Time* time = Time::get();

        struct timespec ts;
        timespec_get(&ts, TIME_UTC);

        int64_t millis = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;

        //std::cout << "Broadcasting timestamp: " << millis << std::endl;

        auto app = WebsocketServer::get()->get_app();
        //app->publish("broadcast", std::string_view((char*)&millis, sizeof(millis)), uWS::OpCode::BINARY, false);

        time->update();

        system->update(Time::get()->get_deltatime());
        window->update();
        engine->update();
        window->post_update();
        }, 8, 8);

      if (!m_app) {
        std::cerr << "SSLApp is not initialized!" << std::endl;
      }

      app.run();
    }

    static WebsocketServer* get() {
      return m_instance;
    }

    uWS::SSLApp* get_app() {
      return m_app;
    }

  private:
    int m_port;
    int m_threads_length;

    uWS::SSLApp* m_app = nullptr;

    static WebsocketServer* m_instance;
    std::vector<std::thread> m_thread_pool;
  };



} // namespace astralix
