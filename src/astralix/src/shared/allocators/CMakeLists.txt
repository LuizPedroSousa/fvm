file(GLOB_RECURSE SHARED_ALLOCATORS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE SHARED_ALLOCATORS_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

list(FILTER SHARED_ALLOCATORS_SRC EXCLUDE REGEX ".*\\.test\\.cpp$")
list(FILTER SHARED_ALLOCATORS_HEADERS EXCLUDE REGEX ".*\\.test\\.hpp$")

add_library(allocators STATIC ${SHARED_ALLOCATORS_SRC}
                              ${SHARED_ALLOCATORS_HEADERS})

add_library(shared::allocators ALIAS allocators)

target_include_directories(allocators PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(allocators PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                                        shared::foundation)
