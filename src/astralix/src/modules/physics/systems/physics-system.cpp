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
inline glm::mat4 PxQuatToGlmRotationMatrix(const physx::PxQuat &pxQuat) {
  glm::quat glmQuat = glm::quat(pxQuat.w, pxQuat.x, pxQuat.y, pxQuat.z);
  return glm::mat4_cast(glmQuat); // Converts quaternion to rotation matrix
}

inline glm::mat4 PxMat44ToGlmMat4(physx::PxMat44 pxMatrix) {
  glm::mat4 matrix;
  for (int x = 0; x < 4; x++)
    for (int y = 0; y < 4; y++)
      matrix[x][y] = pxMatrix[x][y];
  return matrix;
}

inline PxQuat GlmQuatToPxQuat(glm::quat quat) {
  return {quat.x, quat.y, quat.z, quat.w};
}

inline physx::PxMat44 GlmMat4ToPxMat44(glm::mat4 glmMatrix) {
  physx::PxMat44 matrix;
  for (int x = 0; x < 4; x++)
    for (int y = 0; y < 4; y++)
      matrix[x][y] = glmMatrix[x][y];
  return matrix;
}

inline PxShape *create_box_shape(float width, float height, float depth,
                                 glm::mat4 transform_matrix) {

  auto _defaultMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

  PxShape *shape = gPhysics->createShape(PxBoxGeometry(width, height, depth),
                                         *_defaultMaterial, true);

  PxMat44 localShapeMatrix = GlmMat4ToPxMat44(transform_matrix);

  PxTransform localShapeTransform(localShapeMatrix);
  shape->setLocalPose(localShapeTransform);

  return shape;
}

class TransformSimulationEventCallback : PxSimulationEventCallback {

  virtual void onConstraintBreak(PxConstraintInfo *constraints,
                                 PxU32 count) override {};

  /**
  \brief This is called with the actors which have just been woken up.

  \note Only supported by rigid bodies yet.
  \note Only called on actors for which the PxActorFlag eSEND_SLEEP_NOTIFIES has
  been set.
  \note Only the latest sleep state transition happening between fetchResults()
  of the previous frame and fetchResults() of the current frame will get
  reported. For example, let us assume actor A is awake, then A->putToSleep()
  gets called, then later A->wakeUp() gets called. At the next
  simulate/fetchResults() step only an onWake() event will get triggered because
  that was the last transition.
  \note If an actor gets newly added to a scene with properties such that it is
  awake and the sleep state does not get changed by the user or simulation, then
  an onWake() event will get sent at the next simulate/fetchResults() step.

  \param[in] actors - The actors which just woke up.
  \param[in] count  - The number of actors

  \see PxScene.setSimulationEventCallback() PxSceneDesc.simulationEventCallback
  PxActorFlag PxActor.setActorFlag()
  */
  virtual void onWake(PxActor **actors, PxU32 count) override {};

  /**
  \brief This is called with the actors which have just been put to sleep.

  \note Only supported by rigid bodies yet.
  \note Only called on actors for which the PxActorFlag eSEND_SLEEP_NOTIFIES has
  been set.
  \note Only the latest sleep state transition happening between fetchResults()
  of the previous frame and fetchResults() of the current frame will get
  reported. For example, let us assume actor A is asleep, then A->wakeUp() gets
  called, then later A->putToSleep() gets called. At the next
  simulate/fetchResults() step only an onSleep() event will get triggered
  because that was the last transition (assuming the simulation does not wake
  the actor up).
  \note If an actor gets newly added to a scene with properties such that it is
  asleep and the sleep state does not get changed by the user or simulation,
  then an onSleep() event will get sent at the next simulate/fetchResults()
  step.

  \param[in] actors - The actors which have just been put to sleep.
  \param[in] count  - The number of actors

  \see PxScene.setSimulationEventCallback() PxSceneDesc.simulationEventCallback
  PxActorFlag PxActor.setActorFlag()
  */
  virtual void onSleep(PxActor **actors, PxU32 count) override {};

  /**
  \brief This is called when certain contact events occur.

  The method will be called for a pair of actors if one of the colliding shape
  pairs requested contact notification. You request which events are reported
  using the filter shader/callback mechanism (see #PxSimulationFilterShader,
  #PxSimulationFilterCallback, #PxPairFlag).

  Do not keep references to the passed objects, as they will be
  invalid after this function returns.

  \param[in] pairHeader Information on the two actors whose shapes triggered a
  contact report.
  \param[in] pairs The contact pairs of two actors for which contact reports
  have been requested. See #PxContactPair.
  \param[in] nbPairs The number of provided contact pairs.

  \see PxScene.setSimulationEventCallback() PxSceneDesc.simulationEventCallback
  PxContactPair PxPairFlag PxSimulationFilterShader PxSimulationFilterCallback
  */
  virtual void onContact(const PxContactPairHeader &pairHeader,
                         const PxContactPair *pairs, PxU32 nbPairs) override {};

  /**
  \brief This is called with the current trigger pair events.

  Shapes which have been marked as triggers using PxShapeFlag::eTRIGGER_SHAPE
  will send events according to the pair flag specification in the filter shader
  (see #PxPairFlag, #PxSimulationFilterShader).

  \note Trigger shapes will no longer send notification events for interactions
  with other trigger shapes.

  \param[in] pairs - The trigger pair events.
  \param[in] count - The number of trigger pair events.

  \see PxScene.setSimulationEventCallback() PxSceneDesc.simulationEventCallback
  PxPairFlag PxSimulationFilterShader PxShapeFlag PxShape.setFlag()
  */
  virtual void onTrigger(PxTriggerPair *pairs, PxU32 count) override {};

  /**
  \brief Provides early access to the new pose of moving rigid bodies.

  When this call occurs, rigid bodies having the
  #PxRigidBodyFlag::eENABLE_POSE_INTEGRATION_PREVIEW flag set, were moved by the
  simulation and their new poses can be accessed through the provided buffers.

  \note The provided buffers are valid and can be read until the next call to
  #PxScene::simulate() or #PxScene::collide().

  \note This callback gets triggered while the simulation is running. If the
  provided rigid body references are used to read properties of the object, then
  the callback has to guarantee no other thread is writing to the same body at
  the same time.

  \note The code in this callback should be lightweight as it can block the
  simulation, that is, the #PxScene::fetchResults() call.

  \param[in] bodyBuffer The rigid bodies that moved and requested early pose
  reporting.
  \param[in] poseBuffer The integrated rigid body poses of the bodies listed in
  bodyBuffer.
  \param[in] count The number of entries in the provided buffers.

  \see PxScene.setSimulationEventCallback() PxSceneDesc.simulationEventCallback
  PxRigidBodyFlag::eENABLE_POSE_INTEGRATION_PREVIEW
  */
  virtual void onAdvance(const PxRigidBody *const *bodyBuffer,
                         const PxTransform *poseBuffer,
                         const PxU32 count) override {};
};

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

  //   gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
  //
  //   auto ground = EntityManager::get()->get_entity_by_name("ground");
  //   auto ground_transform = ground->get_component<TransformComponent>();
  //
  //   PxQuat ground_quat =
  //   GlmQuatToPxQuat(glm::quat(ground_transform->rotation)); PxTransform
  //   ground_trans = PxTransform(PxVec3(ground_transform->position.x,
  //                                                 ground_transform->position.y,
  //                                                 ground_transform->position.z),
  //                                          ground_quat);
  //
  //   PxRigidStatic *ground_body = gPhysics->createRigidStatic(ground_trans);
  //
  //   // You are passing in a PxShape pointer and any shape offset will affects
  //   that
  //   // actually object, wherever the fuck it is up the function chain. Maybe
  //   look
  //   // into this when you can be fucked, possibly you can just set the
  //   isExclusive
  //   // bool to true, where and whenever the fuck that is and happens.
  //   PxFilterData ground_filterData;
  //   ground_filterData.word0 = (PxU32)22;
  //
  //   auto ground_shape =
  //       create_box_shape(1.0f, 1.0f, 1.0f, ground_transform->matrix);
  //
  //   ground_shape->setQueryFilterData(ground_filterData);      // ray casts
  //   ground_shape->setSimulationFilterData(ground_filterData); // collisions
  //
  //   ground_body->attachShape(*ground_shape);
  //
  //   ground_body->userData = ground;
  //   gScene->addActor(*ground_body);
  //
  //   auto block = EntityManager::get()->get_entity_by_name("block");
  //   auto block_transform = block->get_component<TransformComponent>();
  //
  //   PxQuat block_quat =
  //   GlmQuatToPxQuat(glm::quat(block_transform->rotation)); PxTransform
  //   block_trans = PxTransform(PxVec3(block_transform->position.x,
  //                                                block_transform->position.y,
  //                                                block_transform->position.z),
  //                                         block_quat);
  //
  //   PxRigidDynamic *block_body = gPhysics->createRigidDynamic(block_trans);
  //
  //   auto block_shape =
  //       create_box_shape(1.0f, 1.0f, 1.0f, block_transform->matrix);
  //
  //   PxFilterData block_filterData;
  //   block_filterData.word0 = (PxU32)22;
  //
  //   block_shape->setQueryFilterData(block_filterData);      // ray casts
  //   block_shape->setSimulationFilterData(block_filterData); // collisions
  //
  //   block_body->attachShape(*block_shape);
  //   PxRigidBodyExt::updateMassAndInertia(*block_body, 10.0f);
  //
  //   block_body->userData = block;
  //   gScene->addActor(*block_body);
  //
  //   ground_shape->release();

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
  //
  // Get all rigid actors from the scene
  PxU32 actorCount = gScene->getNbActors(PxActorTypeFlag::eRIGID_STATIC |
                                         PxActorTypeFlag::eRIGID_DYNAMIC);

  std::vector<PxActor *> actors(actorCount);
  gScene->getActors(PxActorTypeFlag::eRIGID_STATIC |
                        PxActorTypeFlag::eRIGID_DYNAMIC,
                    actors.data(), actorCount);

  for (PxActor *actor : actors) {
    // Check that userData is set (i.e. the actor is linked to an entity)
    if (actor->userData) {
      // Cast userData back to your entity type. Adjust this cast based on
      // your actual type.
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

      // Convert PhysX quaternion to glm::quat (ensure correct component
      // ordering)
      glm::quat physicsQuat = glm::quat(pose.q.w, pose.q.x, pose.q.y, pose.q.z);

      // Extract rotation angle (in radians) and axis from the quaternion.
      float angle = glm::angle(physicsQuat);   // angle in radians
      glm::vec3 axis = glm::axis(physicsQuat); // normalized axis

      // Update the transform component properties:
      transform->position = physicsPos;
      transform->rotation = axis;
      transform->rotation_angle =
          glm::degrees(angle); // store angle in degrees, as your recalc expects

      // Recalculate the transform matrix using glm::rotate:
      glm::mat4 translation_matrix =
          glm::translate(glm::mat4(1.0f), physicsPos);
      glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0f), transform->scale);
      glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angle, axis);

      transform->matrix = translation_matrix * rotation_matrix * scale_matrix;
      // transform->rotation = PxQuatToGlmQuat(pose.q);

      // Optionally update the matrix if your engine uses one:
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
