#include "object.hpp"
#include "base.hpp"
#include "components/material/material-component.hpp"
#include "components/mesh/mesh-component.hpp"
#include "components/resource/resource-component.hpp"
#include "components/transform/transform-component.hpp"
#include "entities/ientity.hpp"
#include <string>

namespace astralix {

  Object::Object(ENTITY_INIT_PARAMS, glm::vec3 position, glm::vec3 scale)
    : ENTITY_INIT() {
    add_component<ResourceComponent>();
    add_component<TransformComponent>(position, scale);
  }

  void Object::start() {

    CHECK_ACTIVE(this);

    auto resource = get_component<ResourceComponent>();
    auto mesh = get_component<MeshComponent>();
    auto transform = get_component<TransformComponent>();

    if (resource != nullptr && resource->is_active())
      resource->start();

    auto shader = resource->get_shader();

    if (shader != nullptr) {
      shader->bind();
      shader->set_int("shadowMap", 1);
    }

    if (transform != nullptr && transform->is_active())
      transform->start();

    if (mesh != nullptr && mesh->is_active())
      mesh->start();
  }

  void Object::pre_update() {}

  void Object::update() {
    CHECK_ACTIVE(this);

    auto resource = get_component<ResourceComponent>();
    auto mesh = get_component<MeshComponent>();
    auto transform = get_component<TransformComponent>();

    auto material = get_component<MaterialComponent>();

    resource->update();

    if (transform != nullptr && transform->is_active()) {
      transform->update();
    }

    if (material != nullptr && material->is_active()) {
      material->update();
    }
  }

  void Object::post_update() {}

} // namespace astralix
