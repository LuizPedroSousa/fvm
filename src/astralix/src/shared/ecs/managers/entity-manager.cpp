#include "entity-manager.hpp"

namespace astralix {

EntityManager::EntityManager() {}

EntityManager::~EntityManager() {}

void EntityManager::destroy_entity(const EntityID &entity_id) {
  const EntityTypeID type_id = m_entity_table[entity_id]->get_entity_type_id();

  ComponentManager::get()->clean_components(entity_id);

  m_entity_table.erase(entity_id);
}

IEntity *EntityManager::get_entity(const EntityID &entity_id) {
  auto it = m_entity_table.find(entity_id);

  if (it != m_entity_table.end()) {
    return it->second.get();
  }

  return nullptr;
}

}; // namespace astralix