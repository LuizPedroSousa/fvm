cmake_minimum_required(VERSION 3.0)

project(fvm)

add_definitions(-DPX_PHYSX_STATIC_LIB)

if(DEFINED ENV{MODE})
  set(MODE $ENV{MODE})
else()
  set(MODE "DEBUG")
endif()

set(EXPORT_COMPILE_COMMANDS TRUE)

# set(CMAKE_TOOLCHAIN_FILE
# "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING
# "Vcpkg toolchain file")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

add_compile_options(-fPIC)

# add_definitions(-DASTRA_EDITOR)
add_definitions(-DENABLE_LOGS)
add_definitions(-DLOG_TO_CONSOLE)

find_package(OpenGL REQUIRED)
find_package(assimp REQUIRED)

set(VCPKG_INSTALLED_ROOT "${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-linux")
set(jsoncpp_DIR "${VCPKG_INSTALLED_ROOT}/share/jsoncpp")
set(freetype_DIR "${VCPKG_INSTALLED_ROOT}/share/freetype")
set(imgui_DIR "${VCPKG_INSTALLED_ROOT}/share/imgui")
set(imguizmo_DIR "${VCPKG_INSTALLED_ROOT}/share/imguizmo")
set(glfw3_DIR "${VCPKG_INSTALLED_ROOT}/share/glfw3")
set(uwebsockets_DIR "${VCPKG_INSTALLED_ROOT}/share/unofficial-uwebsockets")

# find_path(UWEBSOCKETS_INCLUDE_DIRS
# "${VCPKG_INSTALLED_ROOT}/include/uwebsockets/App.h")
find_path(USOCKET_LIB_A "${VCPKG_INSTALLED_ROOT}/lib/libuSockets.a")
find_package(ZLIB REQUIRED)

# set(boost_DIR "${VCPKG_INSTALLED_ROOT}/share/boost")

# set(gtk4 "${VCPKG_INSTALLED_ROOT}/share/gtk-4.0")
find_package(jsoncpp CONFIG REQUIRED)
find_package(freetype CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

# find_package(boost CONFIG REQUIRED)
find_package(PkgConfig)

find_package(PkgConfig REQUIRED)

find_package(glfw3 CONFIG REQUIRED)

find_package(X11 REQUIRED)

# pkg_check_modules(GTKMM_VARS REQUIRED IMPORTED_TARGET gtkmm-4.0)

# find_package(imguizmo CONFIG REQUIRED) find_package(gtk CONFIG REQUIRED)
if(WIN32)
  set(LIBS glfw3 opengl32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
elseif(UNIX AND NOT APPLE)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
  find_package(OpenGL REQUIRED)
  add_definitions(${OPENGL_DEFINITIONS})

  # note that the order is important for setting the libs use pkg-config --libs
  # $(pkg-config --print-requires --print-requires-private glfw3) in a terminal
  # to confirm
  set(LIBS
      ${GLFW3_LIBRARY}
      X11
      Xrandr
      Xinerama
      Xi
      Xxf86vm
      Xcursor
      GL
      dl
      pthread
      freetype
      ${ASSIMP_LIBRARY})
  set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
elseif(APPLE)
  include_directories(/System/Library/Frameworks)
  find_library(COCOA_LIBRARY Cocoa)
  find_library(OpenGL_LIBRARY OpenGL)
  find_library(IOKit_LIBRARY IOKit)
  find_library(CoreVideo_LIBRARY CoreVideo)
  mark_as_advanced(COCOA_LIBRARY OpenGL_LIBRARY)
  set(APPLE_LIBS ${COCOA_LIBRARY} ${IOKit_LIBRARY} ${OpenGL_LIBRARY}
                 ${CoreVideo_LIBRARY})
  set(APPLE_LIBS ${APPLE_LIBS} ${GLFW3_LIBRARY} ${ASSIMP_LIBRARY}
                 ${FREETYPE_LIBRARIES})
  set(LIBS ${LIBS} ${APPLE_LIBS})
else()
  set(LIBS)
endif(WIN32)

add_executable(fvm src/main.cpp)

include_directories(
  ${OPENGL_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${ASSIMP_INCLUDE_DIRS}
  ${GTK4_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/external/PhysX/physx/include)

link_directories(external/PhysX/physx/bin/linux.x86_64/release)

add_library(glad "external/glad/glad.c")
add_library(glm "external/glm/detail/glm.cpp")
add_library(stb_image "external/stb_image/stb_image.cpp")

add_subdirectory(${PROJECT_SOURCE_DIR}/external/imgui/impl)
set(LIBS ${LIBS} glad)

set(LIB_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external
                    ${VCPKG_INSTALLED_ROOT}/include)

add_subdirectory(src/astralix/src/shared)
add_subdirectory(src/astralix/src/modules/window)
add_subdirectory(src/astralix/src/modules/renderer)
add_subdirectory(src/astralix/src/modules/editor)
add_subdirectory(src/astralix/src/modules/physics)
add_subdirectory(src/astralix/src/modules/engine)
add_subdirectory(src/astralix/src/modules/application)
add_subdirectory(src/astralix/src/modules/project)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/game)

include_directories(${LIB_INCLUDE_DIR})
link_directories(${VCPKG_INSTALLED_ROOT}/lib)

target_link_libraries(
  fvm
  PRIVATE
    game
    editor
    application
    "${VCPKG_INSTALLED_ROOT}/lib/libuSockets.a"
    ZLIB::ZLIB
    ${PROJECT_SOURCE_DIR}/external/PhysX/physx/bin/linux.x86_64/release/libPhysXFoundation_static_64.a
)
