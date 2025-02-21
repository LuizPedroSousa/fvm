#pragma once
#include "base.hpp"
#include "ecs/guid.hpp"
#include "resource.hpp"
#include "vector"
#include <optional>

namespace astralix {

class Material : public Resource {
public:
  std::vector<ResourceID> diffuses;
  std::vector<ResourceID> speculars;
  std::optional<ResourceID> normal_map;

  Material(RESOURCE_INIT_PARAMS, std::vector<ResourceID> diffuse,
           std::vector<ResourceID> specular,
           std::optional<ResourceID> normal_map);

  static Ref<Material>
  create(ResourceID id, std::vector<ResourceID> diffuse_ids = {},
         std::vector<ResourceID> specular_ids = {},
         std::optional<ResourceID> normal_map = std::nullopt);
};

} // namespace astralix
