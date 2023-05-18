file(GLOB_RECURSE IMGUI_SRC
    ${PROJECT_SOURCE_DIR}/external/imgui/imgui.cpp
    ${PROJECT_SOURCE_DIR}/external/imgui/imgui_internal.cpp
    ${PROJECT_SOURCE_DIR}/external/imgui/imgui_tables.cpp
    ${PROJECT_SOURCE_DIR}/external/imgui/imgui_widgets.cpp
    ${PROJECT_SOURCE_DIR}/external/imgui/imgui_draw.cpp
)
file(GLOB_RECURSE IMGUI_HEADERS
    ${PROJECT_SOURCE_DIR}/external/imgui/imgui.h
    ${PROJECT_SOURCE_DIR}/external/imgui/imgui_internal.h
)

add_library(imgui STATIC ${IMGUI_SRC} ${IMGUI_HEADERS})

include_directories(${PROJECT_SOURCE_DIR}/external)

target_include_directories(imgui PUBLIC ${PROJECT_SOURCE_DIR}/external/imgui)


target_link_libraries(imgui PRIVATE ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES})


