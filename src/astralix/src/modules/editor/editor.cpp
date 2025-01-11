#include "editor.hpp"
#include "layers/systems/layer-system.hpp"
#include "ecs/managers/system-manager.hpp"
#include "time.hpp"
#include "layouts/default-layout.hpp"

namespace astralix {
  Editor* Editor::m_instance = nullptr;

  void Editor::init() {
    if (m_instance == nullptr) {
      m_instance = new Editor;
    }
  }

  void Editor::end() { delete m_instance; }

  Editor::Editor() {
    m_current_layout = create_ref<DefaultLayout>();
  }


  void Editor::start() {
    auto system_manager = SystemManager::get();
    system_manager->add_system<LayerSystem>();
  }

  void Editor::update() {
  }
} // namespace astralix
