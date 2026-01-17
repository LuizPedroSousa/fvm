cmake_minimum_required(VERSION 3.11)

project(sandbox LANGUAGES CXX)

# set(VCPKG_ROOT "${CMAKE_SOURCE_DIR}/../../vcpkg_installed/x64-linux")
# set(ASTRA_INSTALL "${CMAKE_SOURCE_DIR}/../../install")
message(${CMAKE_PREFIX_PATH})
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

# list(APPEND CMAKE_PREFIX_PATH "${ASTRA_INSTALL}" "${VCPKG_ROOT}")

if (NOT TARGET astralix::astra)
  find_package(astralix CONFIG REQUIRED)
endif()

file(GLOB_RECURSE SANDBOX_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE SANDBOX_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)

add_executable(sandbox ${SANDBOX_SRC} ${SANDBOX_HEADERS})

target_include_directories(sandbox PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src) 

target_link_libraries(sandbox PRIVATE 
  astralix::astra)

target_compile_features(sandbox PRIVATE cxx_std_20)

target_compile_options(sandbox PRIVATE
    -Wall -Wextra -Wpedantic
)
