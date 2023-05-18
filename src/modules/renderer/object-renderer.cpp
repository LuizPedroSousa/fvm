#include "object-renderer.hpp"
#include "imgui/imgui.h"
#include "object.hpp"
#include "shader-renderer.hpp"

#include "imgui/impl/imgui_impl_glfw.h"
#include "imgui/impl/imgui_impl_opengl3.h"
#include "window.hpp"

void ObjectRenderer::spawnMany(Object *obj, size_t size) {
  for (size_t i = 0; i < size; i++) {
    this->spawn(obj[i]);
  }
}

void ObjectRenderer::spawn(Object obj) {
  this->objects.push_back(obj);
}

void ObjectRenderer::start_all() {

  for (int i = 0; i < this->objects.size(); i++) {
    this->objects[i].start();
  }
}

static int edit_object_index = -1;

void ObjectRenderer::edit_object() {
  ImGui::SetNextWindowSize(ImVec2(300, 300));
  if (ImGui::BeginPopup("edit")) {
    static bool useSlider = true;

    ImGui::Text("Editing Object %d", edit_object_index);
    ImGui::SameLine();

    if (ImGui::Button("Close")) {
      edit_object_index = -1;
      ImGui::CloseCurrentPopup();
    }

    ImGui::Dummy(ImVec2(0, 20));

    // Position
    ImGui::Text("Position");
    ImGui::SameLine();
    if (useSlider) {
      ImGui::SetNextItemWidth(200);
      ImGui::SliderFloat3("##PositionSlider", (float *)&objects[edit_object_index].position, 0.0f, 10.0f);
    } else {
      ImGui::SetNextItemWidth(200);
      ImGui::DragFloat3("##PositionDrag", (float *)&objects[edit_object_index].position, 0.1f, 0.0f, 10.0f);
    }

    // Scale
    ImGui::Text("Scale");
    ImGui::SameLine();
    if (useSlider) {
      ImGui::SetNextItemWidth(200);
      ImGui::SliderFloat3("##ScaleSlider", (float *)&objects[edit_object_index].scale, 0.0f, 10.0f);
    } else {
      ImGui::SetNextItemWidth(200);
      ImGui::DragFloat3("##ScaleDrag", (float *)&objects[edit_object_index].scale, 0.1f, 0.0f, 10.0f);
    }

    // Rotation
    ImGui::Text("Rotation");
    ImGui::SameLine();
    if (useSlider) {
      ImGui::SetNextItemWidth(200);
      ImGui::SliderFloat3("##RotationSlider", (float *)&objects[edit_object_index].rotation, 0.0f, 10.0f);
    } else {
      ImGui::SetNextItemWidth(200);
      ImGui::DragFloat3("##RotationDrag", (float *)&objects[edit_object_index].rotation, 0.1f, 0.0f, 10.0f);
    }
    ImGui::SliderAngle("##Angle", (float *)&objects[edit_object_index].rotation_angle, 0.0f, 1000.0f);

    if (ImGui::Button("Toggle Input Mode")) {
      useSlider = !useSlider;
    }

    ImGui::EndPopup();
  }
}

void ObjectRenderer::render_all() {

  ImGui::Begin("Objects");

  ImGui::Text("Objects of current scene");
  ImGui::Dummy(ImVec2(0, 20));

  for (int i = 0; i < this->objects.size(); i++) {
    this->objects[i].render();

    ImGui::Text("Object - %d", i);
    ImGui::SameLine();
    if (ImGui::Button(("Edit" + std::to_string(i)).c_str())) {
      edit_object_index = i;
      ImGui::OpenPopup("edit");
    };

    if (ImGui::IsPopupOpen("edit") && edit_object_index == i) {
      edit_object();
    }
  }
}