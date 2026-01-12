#include "scene-hierarchy-layer.hpp"

#include "UI/imgui-widgets.hpp"
#include "managers/layer-manager.hpp"
#include "managers/resource-manager.hpp"

#include "glm/gtc/type_ptr.hpp"

namespace astralix {
void SceneHierarchyLayer::start() {
  // ResourceManager::get()->load_texture(
  //     Texture2D::create("icons::cube", "icons/cube.png"));
}

void SceneHierarchyLayer::update() {
  ImGui::Begin("Scene Hierarchy");

  LayerContext *context = LayerManager::get()->get_layer_context();

  auto manager = EntityManager::get();
  manager->for_each([&](IEntity *entity) {
    ImGuiTreeNodeFlags flags =
        ((context->selected_entity_id == entity->get_entity_id())
             ? ImGuiTreeNodeFlags_Selected
             : 0) |
        ImGuiTreeNodeFlags_OpenOnArrow;
    flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

    bool opened =
        UI::tree_node(entity->name.c_str(), "icons::cube", ImVec2{14, 14},
                      entity->get_entity_id() == context->selected_entity_id);

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
      context->selected_entity_id = entity->get_entity_id();
    }

    if (ImGui::IsItemClicked(ImGuiMouseButton_Right) &&
        context->has_selected_entity()) {
      ImGui::OpenPopup("edit_entity");
    }

    if (opened) {
      ImGui::TreePop();
    }

    if (ImGui::BeginPopup("edit_entity")) {
      if (ImGui::MenuItem("Delete")) {
        EntityManager::get()->destroy_entity(entity->get_entity_id());
        ImGui::CloseCurrentPopup();
      }

      if (ImGui::MenuItem("Add component")) {
        UI::entity_components();
      }

      ImGui::EndPopup();
    }
  });

  if (ImGui::BeginPopupContextWindow(0, 1)) {
    UI::entity_templates();

    ImGui::EndPopup();
  }

  ImGui::End();
}

} // namespace astralix
