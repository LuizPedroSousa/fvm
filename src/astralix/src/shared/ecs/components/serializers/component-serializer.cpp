#include "component-serializer.hpp"
#include "ecs/components/icomponent.hpp"

namespace astralix {
  Json::Value ComponentSerializer::serialize() {
    Json::Value root;

    root["name"] = m_component->name;
    root["owner_id"] = (std::string)m_component->m_owner_id;
    root["active"] = m_component->m_active;
    root["component_id"] = (std::string)m_component->m_component_id;

    return root;
  }

} // namespace astralix
