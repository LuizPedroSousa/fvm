#include "entities/layer.hpp"
#include "window.hpp"

class BackgroundLayer : public Entity<BackgroundLayer>, public Layer {
  public:
  BackgroundLayer(ENTITY_INIT_PARAMS) : ENTITY_INIT() {
  }

  void on_enable() override{};
  void on_disable() override{};

  void start() override{};
  void update() override {

    ImGui::Begin("Background");

    ImGui::Text("Edit the bellow background");

    ImGui::ColorEdit3("Background Color", (float *)Window::get_clear_color());

    float frameTime = 1000.0f / ImGui::GetIO().Framerate;
    float fps = ImGui::GetIO().Framerate;
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", frameTime, fps);

    ImGui::End();
  };
};