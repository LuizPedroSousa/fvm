#pragma once
#include "resource.hpp"
#include "utils/guid.hpp"
#include "vector"

class Material : public Resource {
  public:
  std::vector<ResourceID> diffuses;
  std::vector<ResourceID> speculars;

  Material(RESOURCE_INIT_PARAMS, std::vector<ResourceID> diffuse, std::vector<ResourceID> specular);

  static Material create(ResourceID id, std::vector<ResourceID> diffuse_ids, std::vector<ResourceID> specular_ids);
};