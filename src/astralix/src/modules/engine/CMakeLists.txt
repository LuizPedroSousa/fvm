file(GLOB_RECURSE ENGINE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE ENGINE_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

add_library(engine STATIC ${ENGINE_SRC} ${ENGINE_HEADERS})

include_directories(${CMAKE_SOURCE_DIR}/external)

target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(engine ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} imgui::imgui imgui_impl shared physics renderer application)