# Define os arquivos-fonte e cabeçalhos do módulo
file(GLOB_RECURSE RENDERER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE RENDERER_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

# Cria a biblioteca estática do módulo
add_library(renderer STATIC ${RENDERER_SRC} ${RENDERER_HEADER})

# Inclui o diretório "src" do projeto no caminho de inclusão do compilador
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../shared)
include_directories(${PROJECT_SOURCE_DIR}/external)

# Define os alvos que a biblioteca do módulo depende
target_include_directories(renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                                           ${ASSIMP_INCLUDE_DIRS})

# add_definitions(-DTRACY_ENABLE)

target_link_libraries(
  renderer
  glad::glad
  ${OPENGL_LIBRARIES}
  ${GLFW_LIBRARIES}
  assimp
  engine
  window
  application
  project
  glm
  stb_image
  shared
  imgui::imgui
  imgui_impl
  Freetype::Freetype)
