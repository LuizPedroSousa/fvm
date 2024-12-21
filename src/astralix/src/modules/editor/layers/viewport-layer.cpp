#include "viewport-layer.hpp"
#include "engine.hpp"
#include "glad/glad.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_stdlib.h"
#include "iostream"

namespace astralix {

  void ViewportLayer::start() {};

  void ViewportLayer::update() {
    auto engine = Engine::get();

    // Resize
    FramebufferSpecification spec = engine->framebuffer->get_specification();

    bool can_be_resized =
      m_viewport_size.x > 0.0f &&
      m_viewport_size.y > 0.0f && // zero sized framebuffer is invalid
      (spec.width != m_viewport_size.x || spec.height != m_viewport_size.y);

    if (can_be_resized) {
      engine->framebuffer->resize((uint32_t)m_viewport_size.x,
        (uint32_t)m_viewport_size.y);
    }

    ImGui::Begin("Viewport");
    auto viewport_min_region = ImGui::GetWindowContentRegionMin();
    auto viewport_max_region = ImGui::GetWindowContentRegionMax();
    auto viewport_offset = ImGui::GetWindowPos();

    m_viewport_bounds[0] = { viewport_min_region.x + viewport_offset.x,
                            viewport_min_region.y + viewport_offset.y };
    m_viewport_bounds[1] = { viewport_max_region.x + viewport_offset.x,
                            viewport_max_region.y + viewport_offset.y };

    m_viewport_focused = ImGui::IsWindowFocused();
    m_viewport_hovered = ImGui::IsWindowHovered();

    ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
    m_viewport_size = { viewport_panel_size.x, viewport_panel_size.y };

    ImGui::Image(
      engine->framebuffer->get_color_attachment_id(),
      ImVec2{ m_viewport_size.x, m_viewport_size.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

    ImGui::End();
  };

} // namespace astralix
