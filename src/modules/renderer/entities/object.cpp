#include "object.hpp"
#include "GLFW/glfw3.h"
#include "components/material-component.hpp"
#include "components/mesh-component.hpp"
#include "components/render-component.hpp"
#include "components/resource-component.hpp"
#include "components/transform-component.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "window.hpp"

Object::Object(ENTITY_INIT_PARAMS, glm::vec3 position, glm::vec3 scale) : Entity(id, component_manager) {
  add_component<RenderComponent>();
  add_component<MeshComponent>();
  auto transform = add_component<TransformComponent>(position, scale);

  add_component<ResourceComponent>();
  add_component<MaterialComponent>();
}
