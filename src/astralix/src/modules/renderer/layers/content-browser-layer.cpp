#include "content-browser-layer.hpp"
#include "UI/imgui-widgets.hpp"
#include "codecvt"
#include "iostream"
#include "locale"

#include "engine.hpp"

namespace astralix {

static std::filesystem::path view_directory;

ContentBrowserLayer::ContentBrowserLayer()
    : Layer("Content Browser"),
      m_base_directory(
          std::filesystem::current_path().parent_path().append("src").append(
              "assets")

              ),
      m_current_directory(m_base_directory) {
  view_directory = m_current_directory;
}

void ContentBrowserLayer::start() {
  auto resource_manager = ResourceManager::get();

  m_directory_icon_id =
      resource_manager
          ->load_texture(Texture2D::create("icons::directory",
                                           "icons/directory.png", true))
          ->get_renderer_id();

  m_file_icon_id = resource_manager
                       ->load_texture(Texture2D::create("icons::file",
                                                        "icons/file.png", true))
                       ->get_renderer_id();
};

void ContentBrowserLayer::update() {
  ImGui::Begin(this->name.c_str());

  float aside_width = 250.0f;
  float content_width = ImGui::GetContentRegionAvail().x - aside_width - 20.0f;

  ImGui::BeginChild("Aside", ImVec2(aside_width, 0), true);
  draw_aside_directory_view();
  ImGui::EndChild();

  ImGui::SameLine();

  float content_height =
      ImGui::GetContentRegionAvail().y - ImGui::GetTextLineHeightWithSpacing();

  ImGui::BeginChild("Content", ImVec2(content_width, 0), true);

  draw_content_header();
  draw_directory_view();

  ImGui::EndChild();

  ImGui::End();
};

void ContentBrowserLayer::draw_content_header() {
  ImGui::BeginGroup();
  std::filesystem::path current_path = m_base_directory;
  std::filesystem::path relative_path =
      m_current_directory.lexically_relative(m_base_directory.parent_path());

  ImGuiStyle &style = ImGui::GetStyle();

  ImGuiIO &io = ImGui::GetIO();
  auto bold_font = io.Fonts->Fonts[0];

  for (auto path = relative_path.begin(); path != relative_path.end(); path++) {
    std::string directory_name = path->string();

    ImGui::Text(directory_name.c_str());

    if (std::next(path) != relative_path.end()) {

      ImGui::SameLine();

      ImGui::PushStyleColor(ImGuiCol_Text, style.Colors[ImGuiCol_Separator]);

      ImGui::PushFont(bold_font);
      ImGui::Text("-");
      ImGui::PopFont();
      ImGui::PopStyleColor();

      ImGui::SameLine();
    }
  }

  ImGui::EndGroup();
  ImGui::Dummy(ImVec2(0, 4));
}

void ContentBrowserLayer::draw_sub_directories(
    std::filesystem::path directory) {

  for (auto &directory_entry : std::filesystem::directory_iterator(directory)) {

    if (!directory_entry.is_directory()) {
      continue;
    }

    const auto &path = directory_entry.path();
    std::string filename = path.filename().string();

    auto on_open_directory = [&](auto toggle) {
      toggle();
      m_current_directory = path;
    };

    auto on_close_directory = [&](auto toggle) {
      if (directory_entry.is_directory()) {
        m_current_directory = path.parent_path();
      }
    };

    auto has_opened = UI::tree_node(filename.c_str(), "icons::directory",
                                    ImVec2(14, 14), m_current_directory == path,
                                    on_open_directory, on_close_directory);

    if (has_opened) {
      draw_sub_directories(directory_entry);

      ImGui::TreePop();
    }
  }
}

void ContentBrowserLayer::draw_aside_directory_view() {
  auto has_opened = UI::tree_node(
      "assets", "icons::directory", ImVec2(14, 14),
      m_current_directory == m_base_directory,
      [&](auto toggle) {
        toggle();
        m_current_directory = m_base_directory;
      },
      [&](auto toggle) {
        toggle();
        m_current_directory = m_base_directory;
      },
      {0, 1, 1, 0});

  if (has_opened) {
    draw_sub_directories(m_base_directory);
    ImGui::TreePop();
  }
}

void ContentBrowserLayer::draw_directory_view() {
  static float padding = 16.0f;
  static float thumbnail_size = 64.0f;
  float cell_size = thumbnail_size + padding;

  float panel_width = ImGui::GetContentRegionAvail().x;

  int columns_count =
      (int)((panel_width < 0 ? cell_size : panel_width) / cell_size);

  ImGui::Columns(columns_count < 1 ? 1 : columns_count, 0, false);

  for (auto &directory_entry :
       std::filesystem::directory_iterator(m_current_directory)) {
    const auto &path = directory_entry.path();
    std::string filename = path.filename().string();

    void *icon = reinterpret_cast<void *>(
        directory_entry.is_directory() ? m_directory_icon_id : m_file_icon_id);

    ImGui::PushID(filename.c_str());

    auto transparent_color = ImVec4(0, 0, 0, 0);
    ImGui::PushStyleColor(ImGuiCol_Button, transparent_color);

    ImGui::ImageButton(icon, {thumbnail_size, thumbnail_size}, {0, 1}, {1, 0});

    if (ImGui::BeginDragDropSource()) {
      std::filesystem::path relative_path(path);

      std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
      std::wstring wide_item_path =
          converter.from_bytes(relative_path.string());

      const wchar_t *item_path = wide_item_path.c_str();

      ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", item_path,
                                (wcslen(item_path) + 1) * sizeof(wchar_t));

      ImGui::EndDragDropSource();
    }

    ImGui::PopStyleColor();
    if (ImGui::IsItemHovered() &&
        ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
      if (directory_entry.is_directory())
        m_current_directory /= path.filename();
    }
    ImGui::TextWrapped(filename.c_str());

    ImGui::NextColumn();

    ImGui::PopID();
  }
}

} // namespace astralix
