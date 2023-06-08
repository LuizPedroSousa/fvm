#pragma once
#include "either.hpp"
#include "utils/guid.hpp"

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