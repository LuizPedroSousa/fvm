#pragma once
#include "entities/overlay.hpp"
#include "filesystem"
#include "functional"
#include "tuple"
#include "vector"

namespace astralix {

  class DockspaceOverlay : public Overlay {
  public:
    DockspaceOverlay() : Overlay("Dockspace") {}
    ~DockspaceOverlay();

    void start() override;

    void pre_update() override;
    void update() override;
    void post_update() override;

  private:
    static void draw_menu(const char* title,

      std::function<void()> callback);

    static void draw_menu_bar();
    static void draw_menu_item(const char* title, std::function<void()> callback);
    static void draw_sub_menu(
      const char* title,
      std::vector<std::tuple<const char*, std::function<void()>>> items);

    void configure_imgui_style();

    static ImVec4 hex_to_opengl_rgb(std::string&& hex_color);

    std::filesystem::path get_path(const char* filename);
    void set_theme_colors();
  };

} // namespace astralix