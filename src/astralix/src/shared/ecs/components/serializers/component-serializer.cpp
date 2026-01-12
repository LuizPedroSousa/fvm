#include "component-serializer.hpp"
#include "components/icomponent.hpp"
#include "serialization-context.hpp"

namespace astralix {
void ComponentSerializer::serialize() {
  SerializationContext &ctx = *m_ctx.get();

  ctx["id"] = (std::string)m_component->m_component_id;
  ctx["name"] = m_component->name;
  ctx["owner_id"] = (std::string)m_component->m_owner_id;
  ctx["active"] = m_component->m_active;
}

} // namespace astralix
