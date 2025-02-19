#include "physics-system.hpp"
#include "PxShape.h"
#include "PxSimulationEventCallback.h"
#include "assert.hpp"
#include "components/resource/resource-component.hpp"
#include "components/rigidbody/rigidbody-component.hpp"
#include "components/transform/transform-component.hpp"
#include "ecs/managers/entity-manager.hpp"

#include "engine.hpp"
#include "entities/object.hpp"
#include "events/key-event.hpp"
#include "iostream"

#include "PxPhysics.h"
#include "PxPhysicsAPI.h"
#include "foundation/PxFoundation.h"

#include "foundation/PxQuat.h"
#include "foundation/PxSimpleTypes.h"
#include "foundation/PxVec3.h"

#include <ctype.h>

#include "foundation/PxPreprocessor.h"

using namespace physx;

static PxDefaultAllocator gAllocator;
static PxDefaultErrorCallback gErrorCallback;
static PxFoundation *gFoundation = NULL;
static PxPhysics *gPhysics = NULL;
static PxDefaultCpuDispatcher *gDispatcher = NULL;
static PxScene *gScene = NULL;
static PxMaterial *gMaterial = NULL;
static PxPvd *gPvd = NULL;
static bool gSimulate = false;
#define PVD_HOST "127.0.0.1"

namespace astralix {

PhysicsSystem::PhysicsSystem() {};

void PhysicsSystem::start() {
  auto entity_manager = EntityManager::get();

  gFoundation =
      PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

  ASTRA_ASSERT_THROW(!gFoundation, "[PHYSICS SYSTEM] foundation failed!")

  gPvd = PxCreatePvd(*gFoundation);
  PxPvdTransport *transport =
      PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
  gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

  gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation,
                             PxTolerancesScale(), true, gPvd);

  PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
  sceneDesc.gravity = PxVec3(0.0f, -3.0f, 0.0f);
  gDispatcher = PxDefaultCpuDispatcherCreate(2);
  sceneDesc.cpuDispatcher = gDispatcher;
  sceneDesc.filterShader = PxDefaultSimulationFilterShader;
  gScene = gPhysics->createScene(sceneDesc);

  PxPvdSceneClient *pvdClient = gScene->getScenePvdClient();

  if (pvdClient) {
    pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
    pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
    pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
  }

  entity_manager->for_each<Object>([](Object *object) {
    auto rigid_body_component = object->get_component<RigidBodyComponent>();

    if (rigid_body_component != nullptr) {
      rigid_body_component->start(gPhysics, gScene);
    }
  });

  auto event_dispatcher = EventDispatcher::get();

  event_dispatcher->attach<KeyboardListener, KeyReleasedEvent>(
      [&](KeyReleasedEvent *event) {
        switch (event->key_code) {
        case KeyCode::F4: {
          gSimulate = true;
          break;
        }

        default:
          return;
        }
      });
}

void PhysicsSystem::fixed_update(double fixed_dt) {

  if (!gSimulate)
    return;
  gScene->simulate(fixed_dt);
  gScene->fetchResults(true);

  PxU32 actorCount = gScene->getNbActors(PxActorTypeFlag::eRIGID_STATIC |
                                         PxActorTypeFlag::eRIGID_DYNAMIC);

  std::vector<PxActor *> actors(actorCount);
  gScene->getActors(PxActorTypeFlag::eRIGID_STATIC |
                        PxActorTypeFlag::eRIGID_DYNAMIC,
                    actors.data(), actorCount);

  for (PxActor *actor : actors) {
    if (actor->userData) {
      IEntity *entity = static_cast<IEntity *>(actor->userData);

      auto transform = entity->get_component<TransformComponent>();

      if (!transform)
        continue;

      PxTransform pose;
      if (actor->is<PxRigidDynamic>()) {
        pose = static_cast<PxRigidDynamic *>(actor)->getGlobalPose();
      } else {
        pose = static_cast<PxRigidStatic *>(actor)->getGlobalPose();
      }

      glm::vec3 physicsPos(pose.p.x, pose.p.y, pose.p.z);

      glm::quat physicsQuat = glm::quat(pose.q.w, pose.q.x, pose.q.y, pose.q.z);

      float angle = glm::angle(physicsQuat);
      glm::vec3 axis = glm::axis(physicsQuat);

      transform->position = physicsPos;
      transform->rotation = axis;
      transform->rotation_angle = glm::degrees(angle);

      glm::mat4 translation_matrix =
          glm::translate(glm::mat4(1.0f), physicsPos);
      glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0f), transform->scale);
      glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angle, axis);

      transform->matrix = translation_matrix * rotation_matrix * scale_matrix;
    }
  }
};

void PhysicsSystem::pre_update(double dt) {

};

void PhysicsSystem::update(double dt) {
  auto component_manager = ComponentManager::get();
  auto entity_manager = EntityManager::get();

  entity_manager->for_each<Object>([dt](Object *object) {
    auto rigidbody_component = object->get_component<RigidBodyComponent>();

    if (rigidbody_component != nullptr) {
      rigidbody_component->update(dt);
    }
  });
};

PhysicsSystem::~PhysicsSystem() {
  gPhysics->release();
  gFoundation->release();
};
} // namespace astralix
