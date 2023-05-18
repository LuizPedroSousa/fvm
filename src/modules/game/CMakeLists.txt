# Define os arquivos-fonte e cabeçalhos do módulo
file(GLOB_RECURSE GAME_SRC ${PROJECT_SOURCE_DIR}/src/modules/game/*.cpp)
file(GLOB_RECURSE GAME_HEADERS ${PROJECT_SOURCE_DIR}/src/modules/game/*.hpp)

# Cria a biblioteca estática do módulo
Add_library(game STATIC ${GAME_SRC} ${GAME_HEADERS})


# Inclui o diretório "src" do projeto no caminho de inclusão do compilador

include_directories(${PROJECT_SOURCE_DIR}/src/shared)
include_directories(${PROJECT_SOURCE_DIR}/external)

# Define os alvos que a biblioteca do módulo depende
target_include_directories(game PUBLIC ${PROJECT_SOURCE_DIR}/src/modules/game)

target_link_libraries(game ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} renderer window shared glm glad)
