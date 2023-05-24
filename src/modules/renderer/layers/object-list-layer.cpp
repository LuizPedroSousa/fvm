#include "object-list-layer.hpp"
#include "components/transform-component.hpp"
#include "game.hpp"

void ObjectListLayer::start() {
}

static EntityID edit_object_id = -1;

void ObjectListLayer::edit_object(Object *object) {
  ImGui::SetNextWindowSize(ImVec2(300, 300));
  if (ImGui::BeginPopup("edit")) {

    auto transform = object->get_component<TransformComponent>();

    static bool useSlider = true;

    ImGui::Text("Editing Object %d", edit_object_id);
    ImGui::SameLine();

    if (ImGui::Button("Close")) {
      edit_object_id = -1;
      ImGui::CloseCurrentPopup();
    }

    ImGui::Dummy(ImVec2(0, 20));

    // Position
    ImGui::Text("Position");
    ImGui::SameLine();
    if (useSlider) {
      ImGui::SetNextItemWidth(200);
      ImGui::SliderFloat3("##PositionSlider", (float *)transform->get_position(), 0.0f, 10.0f);
    } else {
      ImGui::SetNextItemWidth(200);
      ImGui::DragFloat3("##PositionDrag", (float *)transform->get_position(), 0.1f, 0.0f, 10.0f);
    }

    // Scale
    ImGui::Text("Scale");
    ImGui::SameLine();
    if (useSlider) {
      ImGui::SetNextItemWidth(200);
      ImGui::SliderFloat3("##ScaleSlider", (float *)transform->get_scale(), 0.0f, 10.0f);
    } else {
      ImGui::SetNextItemWidth(200);
      ImGui::DragFloat3("##ScaleDrag", (float *)transform->get_scale(), 0.1f, 0.0f, 10.0f);
    }

    // Rotation
    ImGui::Text("Rotation");
    ImGui::SameLine();
    if (useSlider) {
      ImGui::SetNextItemWidth(200);
      ImGui::SliderFloat3("##RotationSlider", (float *)transform->get_rotation(), 0.0f, 10.0f);
    } else {
      ImGui::SetNextItemWidth(200);
      ImGui::DragFloat3("##RotationDrag", (float *)transform->get_rotation(), 0.1f, 0.0f, 10.0f);
    }
    ImGui::SliderAngle("##Angle", (float *)transform->get_rotation_angle(), 0.0f, 1000.0f);

    if (ImGui::Button("Toggle Input Mode")) {
      useSlider = !useSlider;
    }

    ImGui::EndPopup();
  }
}

void ObjectListLayer::update() {

  ImGui::Begin("Objects");

  ImGui::Text("Objects of current scene");
  ImGui::Dummy(ImVec2(0, 20));

  auto manager = Game::get()->get_entity_manager();
  auto entities = manager->get_entities();

  for (auto entity = entities.begin(); entity != entities.end(); entity++) {
    ImGui::Text("Object - %d", entity->second->get_entity_id());
    ImGui::SameLine();
    if (ImGui::Button(("Edit" + std::to_string(entity->second->get_entity_id())).c_str())) {
      edit_object_id = entity->second->get_entity_id();
      ImGui::OpenPopup("edit");
    };

    if (ImGui::IsPopupOpen("edit") && edit_object_id != -1) {
      edit_object(static_cast<Object *>(manager->get_entity(edit_object_id)));
    }
  }

  ImGui::End();
}