#include "glad/glad.h"

#include "application.hpp"
#include "time.hpp"
#include "window.hpp"
#include "editor.hpp"
#include "engine.hpp"
#include "websocket-server.hpp"

namespace astralix {
  Application* Application::m_instance = nullptr;


  Application* Application::init() {
    m_instance = new Application;


    WebsocketServer::init();
    Time::init();
    SystemManager::init();
    Editor::init();
    Engine::init();
    Window::init();

    // // Read the OpenGL framebuffer into the vector
    // glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, framebuffer.data());

    // // Send the framebuffer data over WebSocket
    // websocketpp::frame::opcode::value op_code = websocketpp::frame::opcode::binary;
    // ws_server.send(ws_server.get_con_from_hdl(websocketpp::connection_hdl()), &framebuffer[0], framebuffer.size(), op_code);

    return m_instance;
  }

  void Application::start() {
    Window::get()->open("FVM Editor", 1920, 1080);
    Engine::get()->start();
    Editor::get()->start();
    SystemManager::get()->start();
    WebsocketServer::get()->start();
  }

  void Application::run() {
    Window* window = Window::get();
    Engine* engine = Engine::get();
    auto system = SystemManager::get();
    Time* time = Time::get();

    //while (window->is_open()) {
    //  time->update();

    //  system->update(Time::get()->get_deltatime());
    //  window->update();
    //  engine->update();
    //  window->post_update();
    //}

    delete m_instance;
  }

  void Application::end() { delete m_instance; }

  Application::~Application() {
    Engine::get()->end();
    Time::get()->end();
    Window::get()->close();
  }

} // namespace astralix
