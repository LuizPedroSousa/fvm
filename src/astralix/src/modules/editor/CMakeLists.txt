file(GLOB_RECURSE EDITOR_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE EDITOR_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

add_library(editor STATIC ${EDITOR_SRC} ${EDITOR_HEADERS})

include_directories(${CMAKE_SOURCE_DIR}/external)
target_include_directories(editor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(
  editor
  glad::glad
  ${OPENGL_LIBRARIES}
  ${GLFW_LIBRARIES}
  imgui::imgui
  # imgui_impl
  shared::foundation
  shared::allocators
  shared::events
  physics
  renderer
  window
  JsonCpp::JsonCpp)
