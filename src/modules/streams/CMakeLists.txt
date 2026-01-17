file(GLOB_RECURSE STREAMS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE STREAMS_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

add_library(streams STATIC ${STREAMS_SRC} ${STREAMS_HEADERS})

target_include_directories(streams 
  PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/astralix/modules/streams>
)
target_link_libraries(
  streams PUBLIC engine JsonCpp::JsonCpp
  project shared::allocators)
