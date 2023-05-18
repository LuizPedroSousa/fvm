# Define os arquivos-fonte e cabeçalhos do módulo
file(GLOB_RECURSE APPLICATION_SRC ${PROJECT_SOURCE_DIR}/src/modules/application/*.cpp)
file(GLOB_RECURSE APPLICATION_HEADER ${PROJECT_SOURCE_DIR}/src/modules/application/*.hpp)

# Cria a biblioteca estática do módulo
add_library(application STATIC ${APPLICATION_SRC} ${APPLICATION_HEADER})


# Inclui o diretório "src" do projeto no caminho de inclusão do compilador

include_directories(${PROJECT_SOURCE_DIR}/src/shared)
include_directories(${PROJECT_SOURCE_DIR}/external)

# Define os alvos que a biblioteca do módulo depende
target_include_directories(application PUBLIC ${PROJECT_SOURCE_DIR}/src/modules/application)

target_link_libraries(application ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} imgui imgui_impl renderer game window shared glad)
