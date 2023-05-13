file(GLOB_RECURSE SHARED_SRC ${PROJECT_SOURCE_DIR}/src/shared/*.cpp)
file(GLOB_RECURSE SHARED_HEADERS ${PROJECT_SOURCE_DIR}/src/shared/*.hpp)

add_library(shared SHARED ${SHARED_SRC} ${SHARED_HEADERS})

target_include_directories(shared PUBLIC ${PROJECT_SOURCE_DIR}/src/shared)
