#include "glad/glad.h"

#include "application.hpp"
#include "window.hpp"

#include "imgui/imgui.h"
#include "imgui/impl/imgui_impl_glfw.h"
#include "imgui/impl/imgui_impl_opengl3.h"

Application *Application::m_instance = nullptr;

Either<BaseException, Unit> Application::init() {
  m_instance = new Application;

  auto has_opened = Window::get()->open(800, 600);

  ASSERT_COMPARE(has_opened);

  Game::init();

  Application::get()->m_game = Game::get();

  auto has_started = Application::get()->m_game->start();

  ASSERT_COMPARE(has_started);

  return Unit();
}

void Application::run() {
  Window *window = Window::get();

  while (window->is_open()) {
    on_update();

    window->update();

    m_game->update();

    window->post_update();
  }
}

void Application::on_update() {
  m_current_frame = glfwGetTime();
  m_deltatime = m_current_frame - m_last_frame;
  m_last_frame = m_current_frame;
}

Application::~Application() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  Window::get()->close();
}
