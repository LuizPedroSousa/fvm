file(GLOB_RECURSE SHARED_ALLOCATORS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE SHARED_ALLOCATORS_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

list(FILTER SHARED_ALLOCATORS_SRC EXCLUDE REGEX ".*\\.test\\.cpp$")
list(FILTER SHARED_ALLOCATORS_HEADERS EXCLUDE REGEX ".*\\.test\\.hpp$")

add_library(allocators STATIC ${SHARED_ALLOCATORS_SRC}
                              ${SHARED_ALLOCATORS_HEADERS})

add_library(shared::allocators ALIAS allocators)

target_include_directories(allocators
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/astralix/shared/allocators>
)

target_link_libraries(allocators 
    PUBLIC 
    shared::foundation)


source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${ALLOCATORS_SOURCES} ${ALLOCATORS_HEADERS})
