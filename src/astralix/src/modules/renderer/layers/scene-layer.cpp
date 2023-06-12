#include "scene-layer.hpp"
#include "components/material/material-component.hpp"
#include "components/transform/transform-component.hpp"
#include "engine.hpp"
#include "entities/light-source.hpp"

namespace astralix {
void SceneLayer::start() {}

static EntityID edit_object_id = -1;

void SceneLayer::edit_entity(IEntity *object) {
  ImGui::SetNextWindowSize(ImVec2(300, 300));
  if (ImGui::BeginPopup("edit")) {
    auto transform = object->get_component<TransformComponent>();

    static bool useSlider = true;

    // Header
    ImGui::Text("Editing Object %d", edit_object_id);
    ImGui::SameLine();

    if (ImGui::Button("Close")) {
      edit_object_id = -1;
      ImGui::CloseCurrentPopup();
    }

    ImGui::Dummy(ImVec2(0, 20));
    // ----

    ImGui::Text("Transform");

    ImGui::Dummy(ImVec2(0, 20));

    ImGui::Text("Position");
    ImGui::SameLine();
    if (useSlider) {
      ImGui::SetNextItemWidth(200);
      ImGui::SliderFloat3("##PositionSlider",
                          (float *)&transform->get_render_transform()->position,
                          -100.0f, 100.0f);
    } else {
      ImGui::SetNextItemWidth(200);
      ImGui::DragFloat3("##PositionDrag",
                        (float *)&transform->get_render_transform()->position,
                        0.1f, -100.0f, 100.0f);
    }

    ImGui::Text("Scale");
    ImGui::SameLine();
    if (useSlider) {
      ImGui::SetNextItemWidth(200);
      ImGui::SliderFloat3("##ScaleSlider",
                          (float *)&transform->get_render_transform()->scale,
                          -100.0f, 100.0f);
    } else {
      ImGui::SetNextItemWidth(200);
      ImGui::DragFloat3("##ScaleDrag",
                        (float *)&transform->get_render_transform()->scale,
                        0.1f, -10.0f, 10.0f);
    }

    ImGui::Text("Rotation");
    ImGui::SameLine();
    if (useSlider) {
      ImGui::SetNextItemWidth(200);
      ImGui::SliderFloat3("##RotationSlider",
                          (float *)&transform->get_render_transform()->rotation,
                          0.0f, 10.0f);
    } else {
      ImGui::SetNextItemWidth(200);
      ImGui::DragFloat3("##RotationDrag",
                        (float *)&transform->get_render_transform()->rotation,
                        0.1f, 0.0f, 10.0f);
    }
    ImGui::SliderAngle(
        "##Angle", (float *)&transform->get_render_transform()->rotation_angle,
        -10000.0f, 10000.0f);

    if (ImGui::Button("Toggle Input Mode")) {
      useSlider = !useSlider;
    }

    if (ImGui::Button("Reset")) {
      transform->reset_render_transform();
    }

    ImGui::EndPopup();
  }
}

void SceneLayer::update() {
  ImGui::Begin("Objects");

  ImGui::Text("Objects of current scene");
  ImGui::Dummy(ImVec2(0, 20));

  auto manager = Engine::get()->get_entity_manager();
  manager->for_each<LightSource>([](LightSource *object) {
    ImGui::Text("Light Source - %d", object->get_entity_id());
    ImGui::SameLine();
    if (ImGui::Button(
            ("Edit" + std::to_string(object->get_entity_id())).c_str())) {
      edit_object_id = object->get_entity_id();
      ImGui::OpenPopup("edit");
    };
  });

  if (ImGui::IsPopupOpen("edit") && edit_object_id != -1) {
    edit_entity(manager->get_entity(edit_object_id));
  }

  ImGui::End();
}

} // namespace astralix