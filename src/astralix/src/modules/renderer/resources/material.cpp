#include "material.hpp"
#include "ecs/guid.hpp"
#include <optional>

namespace astralix {

Material::Material(RESOURCE_INIT_PARAMS, std::vector<ResourceID> diffuses,
                   std::vector<ResourceID> speculars,
                   std::optional<ResourceID> normal_map)
    : RESOURCE_INIT(), diffuses(diffuses), speculars(speculars),
      normal_map(normal_map) {};

Ref<Material> Material::create(ResourceID id,
                               std::vector<ResourceID> diffuse_ids,
                               std::vector<ResourceID> specular_ids,
                               std::optional<ResourceID> normal_map) {
  return create_ref<Material>(id, diffuse_ids, specular_ids, normal_map);
};

} // namespace astralix
