#include "layer-system.hpp"
#include "entities/layer.hpp"
#include "window.hpp"

#include "imgui/imgui.h"
#include "imgui/impl/imgui_impl_glfw.h"
#include "imgui/impl/imgui_impl_opengl3.h"

#include "layers/background-layer.hpp"
#include "layers/object-list-layer.hpp"
#include "layers/scene-layer.hpp"

#include "managers/layer-manager.hpp"

namespace astralix {

LayerSystem::LayerSystem() { LayerManager::init(); }

void LayerSystem::start() {
  // Initialize ImGui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  ImGui_ImplGlfw_InitForOpenGL(Window::get_value(), true);
  ImGui_ImplOpenGL3_Init("#version 330");

  LayerManager::get()->add_layer<BackgroundLayer>();
  LayerManager::get()->add_layer<ObjectListLayer>();
  LayerManager::get()->add_layer<SceneLayer>();
};

void LayerSystem::fixed_update(double fixed_dt){};

void LayerSystem::pre_update(double dt) {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
};

void LayerSystem::update(double dt) {
  auto manager = LayerManager::get();

  manager->for_each([](Layer *layer) {
    if (layer->is_active()) {
      layer->update();
    }
  });
};

void LayerSystem::post_update(double dt) {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
};

LayerSystem::~LayerSystem() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

} // namespace astralix