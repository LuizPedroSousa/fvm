#include "object.hpp"
#include "components/material/material-component.hpp"
#include "components/mesh/mesh-component.hpp"
#include "components/render/render-component.hpp"
#include "components/resource/resource-component.hpp"
#include "components/transform/transform-component.hpp"

namespace astralix {
Object::Object(ENTITY_INIT_PARAMS, glm::vec3 position, glm::vec3 scale)
    : Entity(id, component_manager) {
  add_component<RenderComponent>();
  add_component<MeshComponent>();
  add_component<TransformComponent>(position, scale);
  add_component<ResourceComponent>();
  add_component<MaterialComponent>();
}

} // namespace astralix
