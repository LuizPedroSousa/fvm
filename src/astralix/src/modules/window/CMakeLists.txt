# Define os arquivos-fonte e cabeçalhos do módulo
file(GLOB_RECURSE WINDOW_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE WINDOW_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

# Cria a biblioteca estática do módulo
add_library(window STATIC ${WINDOW_SRC} ${WINDOW_HEADERS})


# Inclui o diretório "src" do projeto no caminho de inclusão do compilador

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../shared)
include_directories(${PROJECT_SOURCE_DIR}/external)

# Define os alvos que a biblioteca do módulo depende
target_include_directories(window PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


target_link_libraries(window PRIVATE ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} application glad shared glfw)