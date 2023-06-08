#include "entity-manager.hpp"

EntityManager::EntityManager(ComponentManager *component_manager) : m_component_manager(component_manager) {
}

EntityManager::~EntityManager() {
}

void EntityManager::destroy_entity(const EntityID &entity_id) {
  const EntityTypeID type_id = m_entity_table[entity_id]->get_entity_type_id();

  m_component_manager->clean_components(entity_id);

  m_entity_typed_table[type_id]->destroy_entity(entity_id);
  m_entity_table.erase(entity_id);
}

IEntity *EntityManager::get_entity(const EntityID &entity_id) {
  return m_entity_table[entity_id].get();
}
