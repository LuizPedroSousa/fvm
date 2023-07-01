file(GLOB_RECURSE SHARED_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE SHARED_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

add_library(shared STATIC ${SHARED_SRC} ${SHARED_HEADERS})
 
target_include_directories(shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} engine)
