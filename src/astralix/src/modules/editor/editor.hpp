#pragma once
#include "base.hpp"
#include "layouts/layout.hpp"

namespace astralix
{


  struct EditorConfig {


  };

  class Editor {
  public:
    static void init();
    void end();


    static Editor* get() { return m_instance; }

    inline const Ref<EditorConfig> get_config() { return m_config; }

    inline const Ref<Layout> get_current_layout() {
      return m_current_layout;
    }

    void start();
    void update();

  private:
    Editor();
    ~Editor() = default;
    Ref<EditorConfig> m_config = nullptr;
    Ref<Layout> m_current_layout = nullptr;
    static Editor* m_instance;
  };
} // namespace astralix
