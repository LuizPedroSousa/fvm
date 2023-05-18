file(GLOB_RECURSE IMGUI_IMPL_SRC
    ${PROJECT_SOURCE_DIR}/external/imgui/impl/imgui_impl_glfw.cpp
    ${PROJECT_SOURCE_DIR}/external/imgui/impl/imgui_impl_opengl3.cpp
)

file(GLOB_RECURSE IMGUI_IMPL_HEADERS
    ${PROJECT_SOURCE_DIR}/external/imgui/impl/imgui_impl_glfw.h
    ${PROJECT_SOURCE_DIR}/external/imgui/impl/imgui_impl_opengl3.cpp
)

add_library(imgui_impl STATIC ${IMGUI_IMPL_SRC} ${IMGUI_IMPL_HEADERS})

include_directories(${PROJECT_SOURCE_DIR}/external)

target_include_directories(imgui_impl PUBLIC ${PROJECT_SOURCE_DIR}/external/imgui/impl)

target_link_libraries(imgui_impl PRIVATE ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} imgui)


