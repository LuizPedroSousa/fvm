file(GLOB_RECURSE SHARED_ECS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE SHARED_ECS_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

list(FILTER SHARED_ECS_SRC EXCLUDE REGEX ".*\\.test\\.cpp$")
list(FILTER SHARED_ECS_HEADERS EXCLUDE REGEX ".*\\.test\\.hpp$")

add_library(ecs STATIC ${SHARED_ECS_SRC} ${SHARED_ECS_HEADERS})

add_library(shared::ecs ALIAS ecs)

target_include_directories(ecs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ecs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} window streams
                                 shared::events)
