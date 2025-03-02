file(GLOB_RECURSE GAME_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE GAME_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

add_library(game STATIC ${GAME_SRC} ${GAME_HEADERS})

include_directories(${PROJECT_SOURCE_DIR}/external)
target_include_directories(game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(
  game
  ${OPENGL_LIBRARIES}
  glad::glad
  ${GLFW_LIBRARIES}
  physics
  application
  engine
  renderer
  window
  shared
  glm
  project)
