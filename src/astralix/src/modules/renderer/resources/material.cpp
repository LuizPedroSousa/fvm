#include "material.hpp"

namespace astralix {

Material::Material(RESOURCE_INIT_PARAMS, std::vector<ResourceID> diffuses,
                   std::vector<ResourceID> speculars)
    : RESOURCE_INIT(), diffuses(diffuses), speculars(speculars){};

Material Material::create(ResourceID id, std::vector<ResourceID> diffuse_ids,
                          std::vector<ResourceID> specular_ids) {
  return Material(id, diffuse_ids, specular_ids);
};

} // namespace astralix