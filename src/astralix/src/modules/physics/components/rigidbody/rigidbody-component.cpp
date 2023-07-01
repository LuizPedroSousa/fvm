#include "rigidbody-component.hpp"
#include "components/transform/transform-component.hpp"
#include "engine.hpp"

namespace astralix {
RigidBodyComponent::RigidBodyComponent(COMPONENT_INIT_PARAMS, float velocity,
                                       float gravity, float acceleration,
                                       float drag, float mass)
    : COMPONENT_INIT(RigidBodyComponent, "RigidBody", true), velocity(velocity),
      gravity(gravity), acceleration(acceleration), drag(drag), mass(mass){

                                                                };

void RigidBodyComponent::update(float dt) {

  auto transform = get_owner()->get_component<TransformComponent>();

  transform->position.y -= gravity * mass * dt;
}

}; // namespace astralix
