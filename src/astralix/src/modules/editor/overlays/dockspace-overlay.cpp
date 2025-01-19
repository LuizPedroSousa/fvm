#include "dockspace-overlay.hpp"

#include "glad/glad.h"
#include "imgui.h"
#include "imgui/impl/imgui_impl_glfw.h"
#include "imgui/impl/imgui_impl_opengl3.h"

#include "imgui.h"

#include "UI/imgui-widgets.hpp"
#include "application.hpp"
#include "managers/scene-manager.hpp"
#include "window.hpp"

#include "editor.hpp"
#include "filesystem"
#include "iostream"
#include "layers/managers/layer-manager.hpp"

namespace astralix {
void DockspaceOverlay::start() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  configure_imgui_style();

  ImGui_ImplGlfw_InitForOpenGL(Window::get_value(), true);
  ImGui_ImplOpenGL3_Init("#version 410");

  auto editor = Editor::get();

  auto current_layout = editor->get_current_layout();

  ImGui::LoadIniSettingsFromMemory(current_layout->ini_settings.c_str());
}

void DockspaceOverlay::update() {
  // Note: Switch this to true to enable dockspace
  static bool dockspace_open = true;
  static bool opt_fullscreen_persistant = true;
  bool opt_fullscreen = opt_fullscreen_persistant;
  static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

  // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window
  // not dockable into, because it would be confusing to have two docking
  // targets within each others.
  ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
  if (opt_fullscreen) {
    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |=
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
  }

  // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render
  // our background and handle the pass-thru hole, so we ask Begin() to not
  // render a background.
  if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
    window_flags |= ImGuiWindowFlags_NoBackground;

  // Important: note that we proceed even if Begin() returns false (aka window
  // is collapsed). This is because we want to keep our DockSpace() active. If a
  // DockSpace() is inactive, all active windows docked into it will lose their
  // parent and become undocked. We cannot preserve the docking relationship
  // between an active window and an inactive docking, otherwise any change of
  // dockspace/settings would lead to windows being stuck in limbo and never
  // being visible.
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::Begin("DockSpace", &dockspace_open, window_flags);
  ImGui::PopStyleVar();

  if (opt_fullscreen)
    ImGui::PopStyleVar(2);

  // DockSpace
  ImGuiIO &io = ImGui::GetIO();
  ImGuiStyle &style = ImGui::GetStyle();
  float minWinSizeX = style.WindowMinSize.x;
  style.WindowMinSize.x = 360.0f;
  style.TabRounding = 0.0f;

  if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
  }

  style.WindowMinSize.x = minWinSizeX;

  draw_menu_bar();

  ImGui::End();
}

void DockspaceOverlay::post_update() {

  ImGuiIO &io = ImGui::GetIO();
  Window *window = Window::get();

  io.DisplaySize =
      ImVec2((float)window->get_width(), (float)window->get_height());

  // Rendering
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  io.ConfigFlags &= ~ImGuiConfigFlags_ViewportsEnable;
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    GLFWwindow *backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
  }
}

void DockspaceOverlay::pre_update() {

  ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  ImGui::PopStyleVar();
}

void DockspaceOverlay::draw_menu_bar() {
  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Save")) {
        SceneManager::get()->get_active_scene()->save();
      }

      if (ImGui::MenuItem("Exit")) {
        Application::get()->end();
      }
      ImGui::EndMenu();
    }

    auto entity_manager = EntityManager::get();
    auto layer_context = LayerManager::get()->get_layer_context();

    UI::draw_menu("GameObject", [&]() { UI::entity_templates(); });

    UI::draw_menu("Component", [&]() { UI::entity_components(); });

    UI::draw_menu("Window", [&]() {
      auto layer_manager = LayerManager::get();
      std::vector<std::tuple<const char *, std::function<void()>>> panels;

      layer_manager->for_each([&](Layer *layer) {
        panels.push_back({layer->name.c_str(), [&] {

                          }});
      });

      UI::draw_sub_menu("Panels", panels);
    });

    ImGui::EndMenuBar();
  }
}

void DockspaceOverlay::configure_imgui_style() {
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard;           // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport /

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  float fontSize = 18.0f;

  io.Fonts->AddFontFromFileTTF(
      get_path("fonts/open-sans/OpenSans-Bold.ttf").c_str(), fontSize);
  io.FontDefault = io.Fonts->AddFontFromFileTTF(
      get_path("fonts/open-sans/OpenSans-Regular.ttf").c_str(), fontSize);

  // When viewports are enabled we tweak WindowRounding/WindowBg so platform
  // windows can look identical to regular ones.
  ImGuiStyle &style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  style.WindowMenuButtonPosition = ImGuiDir_None;

  style.FramePadding.x = 2.0f;
  style.TabRounding = 1.0f;
  style.ScrollbarRounding = 9;
  style.WindowRounding = 2;
  style.GrabRounding = 2;
  style.FrameRounding = 0.0f;
  style.PopupRounding = 0;
  style.ChildRounding = 2;

  set_theme_colors();
}

std::filesystem::path DockspaceOverlay::get_path(const char *filename) {
  auto base_path = std::filesystem::current_path()
                       .parent_path()
                       .append("src")
                       .append("assets")
                       .append(filename);

  return base_path;
};

ImVec4 DockspaceOverlay::hex_to_opengl_rgb(std::string &&hex_color) {
  std::string colorString = hex_color.substr(1); // Remove the '#' symbol
  std::istringstream iss(colorString);

  unsigned int hex_value;
  iss >> std::hex >> hex_value;

  auto red = static_cast<float>((hex_value >> 16) & 0xFF) / 255.0f;
  auto green = static_cast<float>((hex_value >> 8) & 0xFF) / 255.0f;
  auto blue = static_cast<float>(hex_value & 0xFF) / 255.0f;

  return ImVec4(red, green, blue, 1.0f);
}

void DockspaceOverlay::set_theme_colors() {

  auto &style = ImGui::GetStyle();
  auto &colors = style.Colors;
  colors[ImGuiCol_WindowBg] = hex_to_opengl_rgb("#383838");

  // Headers
  colors[ImGuiCol_Header] = hex_to_opengl_rgb("#383838");
  colors[ImGuiCol_HeaderHovered] = hex_to_opengl_rgb("#333232");
  colors[ImGuiCol_HeaderActive] = hex_to_opengl_rgb("#2c5d87");
  // Buttons
  colors[ImGuiCol_Button] = hex_to_opengl_rgb("#585858");
  colors[ImGuiCol_ButtonHovered] = hex_to_opengl_rgb("#676767 ");
  colors[ImGuiCol_ButtonActive] = hex_to_opengl_rgb("#2c5d87");

  colors[ImGuiCol_Border] = ImVec4{0.705882f, 0.478431f, 0.917647f, 1.0f};

  // Frame BG
  colors[ImGuiCol_FrameBg] = hex_to_opengl_rgb("#333232");
  colors[ImGuiCol_FrameBgHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
  colors[ImGuiCol_FrameBgActive] = hex_to_opengl_rgb("#fff");

  // Tabs
  colors[ImGuiCol_Tab] = hex_to_opengl_rgb("#585858");
  colors[ImGuiCol_TabHovered] = hex_to_opengl_rgb("#699791");
  colors[ImGuiCol_TabActive] = hex_to_opengl_rgb("#3c3c3c");
  colors[ImGuiCol_TabUnfocused] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
  colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};

  // Title
  colors[ImGuiCol_TitleBg] = hex_to_opengl_rgb("#282828");
  colors[ImGuiCol_TitleBgActive] = hex_to_opengl_rgb("#333232");
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

  //// Scrollbar
  // colors[ImGuiCol_ScrollbarBg] = hex_to_opengl_rgb("#191919");
  // colors[ImGuiCol_ScrollbarGrab] = hex_to_opengl_rgb("#191919");
  // colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
  // colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);

  colors[ImGuiCol_Separator] = hex_to_opengl_rgb("#191919");
}

DockspaceOverlay::~DockspaceOverlay() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

} // namespace astralix
