#include "component-manager.hpp"

namespace astralix {

void ComponentManager::clean_components(const EntityID entity_id) {
  if (m_entity_component_table.find(entity_id) !=
      m_entity_component_table.end()) {
    auto components = m_entity_component_table.at(entity_id);

    for (auto entity_component = components.begin(), end = components.end();
         entity_component != end;) {

      m_component_table.erase(entity_component->second);

      entity_component = components.erase(entity_component);
    }
  }
}

} // namespace astralix
