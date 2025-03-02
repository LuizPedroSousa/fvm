file(GLOB_RECURSE WINDOW_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE WINDOW_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

add_library(window STATIC ${WINDOW_SRC} ${WINDOW_HEADERS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../shared)
include_directories(${PROJECT_SOURCE_DIR}/external)

target_include_directories(window PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(window PRIVATE application glad::glad shared glfw)
