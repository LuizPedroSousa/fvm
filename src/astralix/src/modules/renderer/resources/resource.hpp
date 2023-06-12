#pragma once
#include "ecs/guid.hpp"
#include "either.hpp"

namespace astralix {

#define RESOURCE_INIT_PARAMS const ResourceID id
#define RESOURCE_INIT() Resource(id)

class Resource {
public:
  Resource(RESOURCE_INIT_PARAMS) { m_resource_id = id; }
  Resource() {}

  inline const ResourceID get_resource_id() { return m_resource_id; };

private:
  ResourceID m_resource_id;
};

} // namespace astralix