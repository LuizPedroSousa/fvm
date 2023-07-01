#include "console-layer.hpp"

#include "engine.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/misc/cpp/imgui_stdlib.h"

namespace astralix {
void ConsoleLayer::start() {

  auto resource_manager = ResourceManager::get();

  resource_manager->load_textures({{"icons::warn", "icons/warn.png"},
                                   {"icons::info", "icons/info.png"},
                                   {"icons::error", "icons/error.png"}});
};

void ConsoleLayer::update() {

  ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar;

  ImGui::Begin("Console", nullptr, window_flags);

  auto resource_manager = ResourceManager::get();

  if (ImGui::BeginMenuBar()) {

    if (ImGui::Button("Clear")) {
      // Adicione aqui a lógica para limpar o console
    }
    ImGui::SameLine();
    static char searchBuffer[256] = "";
    ImGui::InputText("Search", searchBuffer, sizeof(searchBuffer));

    ImGui::SameLine();

    ImVec2 icon_size = ImVec2(14, 14);
    float spacing = 4.0f;

    float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y;

    float icon_position_y =
        ImGui::GetCursorPosY() + (line_height - icon_size.y) * 0.25f;

    ImGui::SameLine();
    ImGui::Text("0");

    ImVec4 clearColor = ImVec4(0.0f, 0.0f, 0.0f, 0.0f); // Transparent color

    auto info_icon = reinterpret_cast<void *>(
        resource_manager->get_texture_by_id("icons::info")->get_id());

    ImGui::PushStyleColor(ImGuiCol_Button,
                          clearColor); // Set the button color to transparent
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, clearColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, clearColor);

    ImGui::SetCursorPosY(icon_position_y);
    if (ImGui::ImageButton(info_icon, icon_size)) {
    }

    ImGui::PopStyleColor(3); // Pop the style colors

    ImGui::SameLine();
    ImGui::SetCursorPosY(icon_position_y);
    ImGui::Text("0");

    ImGui::SameLine(0.0f, spacing);
    ImGui::SetCursorPosY(icon_position_y);
    if (ImGui::ImageButton(
            reinterpret_cast<void *>(
                resource_manager->get_texture_by_id("icons::warn")->get_id()),
            icon_size)) {
    }

    ImGui::SameLine(0.0f, spacing);

    ImGui::SetCursorPosY(icon_position_y);

    ImGui::SameLine();
    ImGui::Text("0");
    if (ImGui::ImageButton(
            reinterpret_cast<void *>(
                resource_manager->get_texture_by_id("icons::error")->get_id()),
            icon_size)) {
    }

    ImGui::EndMenuBar();
  }

  ImGui::End();
}
} // namespace astralix
