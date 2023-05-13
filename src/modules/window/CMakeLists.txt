# Define os arquivos-fonte e cabeçalhos do módulo
file(GLOB_RECURSE WINDOW_SRC ${PROJECT_SOURCE_DIR}/src/modules/window/*.cpp)
file(GLOB_RECURSE WINDOW_HEADERS ${PROJECT_SOURCE_DIR}/src/modules/window/*.hpp)

# Cria a biblioteca estática do módulo
add_library(window SHARED ${WINDOW_SRC} ${WINDOW_HEADERS})


# Inclui o diretório "src" do projeto no caminho de inclusão do compilador

include_directories(${PROJECT_SOURCE_DIR}/src/shared)
include_directories(${PROJECT_SOURCE_DIR}/includes)

# Define os alvos que a biblioteca do módulo depende
target_include_directories(window PUBLIC ${PROJECT_SOURCE_DIR}/src/modules/window)

target_link_libraries(window ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} shared glfw GLAD)
