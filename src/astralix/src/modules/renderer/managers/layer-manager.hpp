#include "ecs/guid.hpp"
#include "ecs/managers/component-manager.hpp"
#include "ecs/managers/entity-manager.hpp"
#include "engine.hpp"
#include "entities/layer.hpp"

namespace astralix {

class LayerSystem;

class LayerManager {
public:
  friend class LayerSystem;

  static void init();

  static LayerManager *get() { return m_instance; };

  template <class T> T &add_layer() {
    return m_entity_manager->add_entity<T>();
  }

private:
  LayerManager();

  Layer *get_layer(EntityID id) {
    IEntity *entity = m_entity_manager->get_entity(id);
    Layer *layer = dynamic_cast<Layer *>(entity);

    return layer;
  }

  void for_each(std::function<void(Layer *)> fn) {
    return m_entity_manager->for_each<Layer>(fn);
  }

  std::vector<Layer *> get_layers() {
    return m_entity_manager->get_entities<Layer>();
  }

  static LayerManager *m_instance;

  EntityManager *m_entity_manager;
  ComponentManager *m_component_manager;
};
} // namespace astralix