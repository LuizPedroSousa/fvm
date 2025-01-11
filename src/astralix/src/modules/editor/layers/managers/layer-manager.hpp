#pragma once
#include "base.hpp"
#include "ecs/guid.hpp"
#include "ecs/managers/component-manager.hpp"
#include "ecs/managers/entity-manager.hpp"
#include "engine.hpp"
#include "entities/layer.hpp"
#include "entities/overlay.hpp"
#include "vector"

namespace astralix {

class LayerSystem;

struct LayerContext {
public:
  EntityID selected_entity_id = -1;

  bool has_selected_entity() { return selected_entity_id != -1; }
};

class LayerManager {
public:
  friend class LayerSystem;

  static void init();

  static LayerManager *get() { return m_instance; };

  template <class T> void add_layer() {
    Scope<T> layer = create_scope<T>();
    m_layers.push_back(std::move(layer));
  };

  template <class T> void add_overlay() {
    Scope<T> overlay = create_scope<T>();
    m_overlays.push_back(std::move(overlay));
  };

  LayerContext *get_layer_context() { return m_context; };

  void for_each(std::function<void(Layer *)> fn) {
    for (int i = 0; i < m_layers.size(); i++) {
      fn(m_layers[i].get());
    };
  };

  void for_each(std::function<void(Overlay *)> fn) {
    for (int i = 0; i < m_overlays.size(); i++) {
      fn(m_overlays[i].get());
    };
  };

private:
  LayerManager();
  ~LayerManager();
  static LayerManager *m_instance;

  LayerContext *m_context;

  std::vector<Scope<Overlay>> m_overlays;
  std::vector<Scope<Layer>> m_layers;
};

} // namespace astralix