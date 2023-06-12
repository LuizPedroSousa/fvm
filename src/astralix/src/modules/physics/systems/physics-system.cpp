#include "physics-system.hpp"
#include "components/mesh-collision/mesh-collision-component.hpp"
#include "components/resource/resource-component.hpp"
#include "components/rigidbody/rigidbody-component.hpp"
#include "components/transform/transform-component.hpp"

#include "engine.hpp"
#include "entities/object.hpp"

namespace astralix {

PhysicsSystem::PhysicsSystem(){};

void PhysicsSystem::start() {
  auto entity_manager = Engine::get()->get_entity_manager();

  entity_manager->for_each<Object>([](Object *object) {
    auto mesh_collision_component =
        object->get_component<MeshCollisionComponent>();

    if (mesh_collision_component != nullptr) {
      mesh_collision_component->start();
    }
  });
}

void PhysicsSystem::fixed_update(double fixed_dt){

};

void PhysicsSystem::pre_update(double dt){

};

void PhysicsSystem::update(double dt) {
  auto component_manager = Engine::get()->get_component_manager();
  auto entity_manager = Engine::get()->get_entity_manager();

  entity_manager->for_each<Object>([dt](Object *object) {
    auto rigidbody_component = object->get_component<RigidBodyComponent>();

    if (rigidbody_component != nullptr) {
      rigidbody_component->update(dt);
    }
  });
};

void PhysicsSystem::post_update(double dt){

};
} // namespace astralix
