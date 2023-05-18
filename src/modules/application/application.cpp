#include "glad/glad.h"

#include "application.hpp"
#include "window.hpp"

#include "imgui/imgui.h"
#include "imgui/impl/imgui_impl_glfw.h"
#include "imgui/impl/imgui_impl_opengl3.h"

Application *Application::m_instance = nullptr;

Either<BaseException, Unit> Application::init() {
  m_instance = new Application;

  auto hasOpened = Window::get()->open(800, 600);

  if (hasOpened.isLeft()) {
    return hasOpened.left();
  }

  Application::get()->m_game = Game();

  Application::get()->m_game.start();

  // Initialize ImGui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  ImGui_ImplGlfw_InitForOpenGL(Window::get_value(), true);
  ImGui_ImplOpenGL3_Init("#version 330");

  return Unit();
}

void Application::run() {
  Window *window = Window::get();

  while (window->is_open()) {
    on_update();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    window->on_update();

    m_game.update();

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    window->on_next();
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
