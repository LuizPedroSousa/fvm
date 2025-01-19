cmake_minimum_required(VERSION 3.0)
project(fvm)

# Include the engine
add_subdirectory(engine)

target_link_libraries(editor PRIVATE engine)

add_executable(runtime src/main.cpp)
target_link_libraries(runtime PRIVATE engine)
