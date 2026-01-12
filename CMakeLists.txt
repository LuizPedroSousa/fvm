cmake_minimum_required(VERSION 3.11)

project(fvm)

add_definitions(-DPX_PHYSX_STATIC_LIB)

set(EXPORT_COMPILE_COMMANDS TRUE)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fopenmp")

add_compile_options(-fPIC)

add_definitions(-DENABLE_LOGS)
add_definitions(-DLOG_TO_CONSOLE)
set(ASTRA_EDITOR false)
set(ASTRA_EDITOR_USOCKET true)
set(ASTRA_ENABLE_TESTS false)
set(ASTRA_TRACE false)

if(ASTRA_EDITOR)
  add_definitions(-DASTRA_EDITOR)
  add_definitions(-DASTRA_EDITOR_USOCKET)
endif()

if(ASTRA_TRACE)
  add_definitions(-DASTRA_TRACE)
  set(TRACE_LIBS "${VCPKG_INSTALLED_ROOT}/lib/libTracyClient.a")
endif()

find_package(OpenGL REQUIRED)
find_package(assimp REQUIRED)

set(VCPKG_INSTALLED_ROOT "${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-linux")

set(jsoncpp_DIR "${VCPKG_INSTALLED_ROOT}/share/jsoncpp")
set(freetype_DIR "${VCPKG_INSTALLED_ROOT}/share/freetype")
set(imgui_DIR "${VCPKG_INSTALLED_ROOT}/share/imgui")
set(imguizmo_DIR "${VCPKG_INSTALLED_ROOT}/share/imguizmo")
set(glfw3_DIR "${VCPKG_INSTALLED_ROOT}/share/glfw3")
set(uwebsockets_DIR "${VCPKG_INSTALLED_ROOT}/share/unofficial-uwebsockets")
set(glad_DIR "${VCPKG_INSTALLED_ROOT}/share/glad")
set(glm_DIR "${VCPKG_INSTALLED_ROOT}/share/glm")

find_path(USOCKET_LIB_A "${VCPKG_INSTALLED_ROOT}/lib/libuSockets.a")
find_path(GLAD_LIB_A "${VCPKG_INSTALLED_ROOT}/lib/libglad.a")
find_package(glad CONFIG REQUIRED)

find_package(OpenMP REQUIRED)

find_package(ZLIB REQUIRED)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

find_package(glm CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)
find_package(freetype CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

find_package(PkgConfig)

find_package(PkgConfig REQUIRED)

find_package(glfw3 CONFIG REQUIRED)

find_package(X11 REQUIRED)

if(ASTRA_ENABLE_TESTS)
  set(GTest_DIR "${VCPKG_INSTALLED_ROOT}/share/gtest")

  enable_testing()

  find_package(GTest CONFIG REQUIRED)

  file(GLOB_RECURSE TEST_SRC ${CMAKE_SOURCE_DIR}/src/**/*.test.cpp)

  add_executable(fvm tests/main.cpp ${TEST_SRC})
else()
  add_executable(fvm src/main.cpp)
endif()

include_directories(
  ${OPENGL_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${ASSIMP_INCLUDE_DIRS}
  ${PROJECT_SOURCE_DIR}/external/PhysX/physx/include)

link_directories(external/PhysX/physx/bin/linux.x86_64/release)

# add_library(glm "external/glm/detail/glm.cpp")
add_library(stb_image "external/stb_image/stb_image.cpp")

# add_subdirectory(${PROJECT_SOURCE_DIR}/external/imgui/impl)
set(LIBS ${LIBS})

set(LIB_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external
                    ${VCPKG_INSTALLED_ROOT}/include)

include_directories(${LIB_INCLUDE_DIR})
link_directories(${VCPKG_INSTALLED_ROOT}/lib)

add_subdirectory(src/astralix/src/shared/foundation)
add_subdirectory(src/astralix/src/shared/allocators)

if(NOT ASTRA_ENABLE_TESTS)
  add_subdirectory(src/astralix/src/shared/events)
  add_subdirectory(src/astralix/src/shared/ecs)
  add_subdirectory(src/astralix/src/modules/streams)
  add_subdirectory(src/astralix/src/modules/project)
  add_subdirectory(src/astralix/src/modules/window)
  add_subdirectory(src/astralix/src/modules/renderer)
  add_subdirectory(src/astralix/src/modules/physics)
  add_subdirectory(src/astralix/src/modules/engine)
  add_subdirectory(src/astralix/src/modules/application)

  add_subdirectory(${CMAKE_SOURCE_DIR}/src/game)
endif()

if(ASTRA_EDITOR)
  add_subdirectory(src/astralix/src/modules/editor)
  set(EDITOR editor)
endif()

if(OpenMP_CXX_FOUND)
  set_target_properties(fvm PROPERTIES COMPILE_FLAGS "${OpenMP_CXX_FLAGS}"
                                       LINK_FLAGS "${OpenMP_CXX_FLAGS}")
endif()

if(ASTRA_ENABLE_TESTS)
  set(BUILD_TESTING OFF)
  add_subdirectory(external/faker-cxx)

  target_link_libraries(
    fvm PRIVATE faker-cxx shared::allocators GTest::gtest GTest::gtest_main
                GTest::gmock GTest::gmock_main)

  include(GoogleTest)
  gtest_discover_tests(fvm)
else()
  target_link_libraries(
    fvm
    PRIVATE
      game
      ${EDITOR}
      application
      ZLIB::ZLIB
      "${VCPKG_INSTALLED_ROOT}/lib/libglad.a"
      ${TRACE_LIBS}
      "${VCPKG_INSTALLED_ROOT}/lib/libuSockets.a"
      ${PROJECT_SOURCE_DIR}/external/PhysX/physx/bin/linux.x86_64/release/libPhysXFoundation_static_64.a
      OpenMP::OpenMP_CXX)
endif()
