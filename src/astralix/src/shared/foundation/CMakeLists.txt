file(GLOB_RECURSE SHARED_FOUNDATION_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE SHARED_FOUNDATION_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

list(FILTER SHARED_FOUNDATION_SRC EXCLUDE REGEX ".*\\.test\\.cpp$")
list(FILTER SHARED_FOUNDATION_HEADERS EXCLUDE REGEX ".*\\.test\\.hpp$")

add_library(foundation STATIC ${SHARED_FOUNDATION_SRC}
                              ${SHARED_FOUNDATION_HEADERS})

add_library(shared::foundation ALIAS foundation)

target_include_directories(foundation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(foundation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
