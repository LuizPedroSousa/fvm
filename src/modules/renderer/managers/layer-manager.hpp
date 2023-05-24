#include "entities/layer.hpp"
#include "game.hpp"
#include "managers/component-manager.hpp"
#include "managers/entity-manager.hpp"
#include "utils/guid.hpp"

class LayerSystem;

class LayerManager {
  public:
  friend class LayerSystem;

  static void init();

  static LayerManager *get() { return m_instance; };

  template <class T>
  T &add_layer() {
    return m_entity_manager->add_entity<T>();
  }

  private:
  LayerManager();

  Layer *get_layer(EntityID id) {
    IEntity *entity = m_entity_manager->get_entity(id);
    Layer *layer = dynamic_cast<Layer *>(entity);

    return layer;
  }

  std::unordered_map<EntityID, IEntity_ptr> get_layers() {
    return m_entity_manager->get_entities();
  }

  static LayerManager *m_instance;

  EntityManager *m_entity_manager;
  ComponentManager *m_component_manager;
};
