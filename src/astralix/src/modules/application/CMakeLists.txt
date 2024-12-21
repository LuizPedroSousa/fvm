# src/headers
file(GLOB_RECURSE APPLICATION_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE APPLICATION_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

add_library(application STATIC ${APPLICATION_SRC} ${APPLICATION_HEADER})

include_directories(${CMAKE_SOURCE_DIR}/external)

target_include_directories(application PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(application ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} editor imgui::imgui imgui_impl engine window shared glad)
