#include "managers/layer-manager.hpp"

LayerManager *LayerManager::m_instance = nullptr;

void LayerManager::init() {
  if (!m_instance)
    m_instance = new LayerManager;
}

LayerManager::LayerManager() : m_component_manager(new ComponentManager()) {
  m_entity_manager = new EntityManager(m_component_manager);
};
