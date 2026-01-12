file(GLOB_RECURSE PHYSICS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE PHYSICS_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

set(PHYSX_RELEASE_PATH
  ${PROJECT_SOURCE_DIR}/external/PhysX/physx/bin/linux.x86_64/release)


set(PHYSX_LIBS
  ${PHYSX_RELEASE_PATH}/libPhysX_static_64.a
  ${PHYSX_RELEASE_PATH}/libPhysXCharacterKinematic_static_64.a
  ${PHYSX_RELEASE_PATH}/libPhysXCommon_static_64.a
  ${PHYSX_RELEASE_PATH}/libPhysXCooking_static_64.a
  ${PHYSX_RELEASE_PATH}/libPhysXExtensions_static_64.a
  ${PHYSX_RELEASE_PATH}/libPhysXFoundation_static_64.a
  ${PHYSX_RELEASE_PATH}/libPVDRuntime_64.so
  ${PHYSX_RELEASE_PATH}/libPhysXPvdSDK_static_64.a
)

if(EXISTS "${PHYSX_RELEASE_PATH}/libPhysXGpu_64.so")
    message(STATUS "PhysX GPU library detected. Enabling CUDA support.")
    list(APPEND PHYSX_LIBS "${PHYSX_RELEASE_PATH}/libPhysXGpu_64.so")
    add_definitions(-DENGINE_USE_PHYSX_GPU)
else()
    message(STATUS "PhysX GPU library NOT found. Building for CPU only.")
endif()

# file(GLOB_RECURSE PHYSX_LIBS ${PHYSX_RELEASE_PATH}/*.a)

add_library(physics STATIC ${PHYSICS_SRC} ${PHYSICS_HEADERS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/shared)
include_directories(${PROJECT_SOURCE_DIR}/external)

link_directories(
  ${PROJECT_SOURCE_DIR}/external/PhysX/physx/bin/linux.x86_64/release)

target_include_directories(physics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(
  physics
  renderer
  shared::foundation
  ${PHYSX_LIBS}
  cuda)
