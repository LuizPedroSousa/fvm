file(GLOB_RECURSE SHARED_EVENTS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE SHARED_EVENTS_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

list(FILTER SHARED_EVENTS_SRC EXCLUDE REGEX ".*\\.test\\.cpp$")
list(FILTER SHARED_EVENTS_HEADERS EXCLUDE REGEX ".*\\.test\\.hpp$")

add_library(events STATIC ${SHARED_EVENTS_SRC} ${SHARED_EVENTS_HEADERS})

add_library(shared::events ALIAS events)

target_include_directories(events 
  PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/astralix/shared/events>
)

target_link_libraries(events PUBLIC shared::ecs)
