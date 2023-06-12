#include "rigidbody-component.hpp"
#include "components/transform/transform-component.hpp"
#include "engine.hpp"

namespace astralix {
RigidBodyComponent::RigidBodyComponent(COMPONENT_INIT_PARAMS, float velocity,
                                       float gravity, float acceleration,
                                       float drag, float mass)
    : COMPONENT_INIT(RigidBodyComponent), velocity(velocity), gravity(gravity),
      acceleration(acceleration), drag(drag), mass(mass){

                                              };

void RigidBodyComponent::update(float dt) {
  auto owner = Engine::get()->get_owner(get_owner_id());

  auto transform = owner->get_component<TransformComponent>();

  transform->get_render_transform()->position.y -= gravity * mass * dt;
}

}; // namespace astralix
