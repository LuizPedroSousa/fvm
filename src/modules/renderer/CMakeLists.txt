# Define os arquivos-fonte e cabeçalhos do módulo
file(GLOB_RECURSE RENDERER_SRC ${PROJECT_SOURCE_DIR}/src/modules/renderer/*.cpp)
file(GLOB_RECURSE RENDERER_HEADER ${PROJECT_SOURCE_DIR}/src/modules/renderer/*.hpp)

# Cria a biblioteca estática do módulo
Add_library(renderer SHARED ${RENDERER_SRC} ${RENDERER_HEADER})


# Inclui o diretório "src" do projeto no caminho de inclusão do compilador

include_directories(${PROJECT_SOURCE_DIR}/src/shared)
include_directories(${PROJECT_SOURCE_DIR}/includes)

# Define os alvos que a biblioteca do módulo depende
target_include_directories(renderer PUBLIC ${PROJECT_SOURCE_DIR}/src/modules/renderer)

target_link_libraries(renderer ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} stb_image shared GLAD)
