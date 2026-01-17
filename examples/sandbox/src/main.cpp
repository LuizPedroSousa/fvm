#include "astralix/modules/application/application.hpp"
#ifdef ASTRA_EDITOR
#else
#endif
#include "astralix/modules/project/managers/project-manager.hpp"
#include "astralix/modules/project/project.hpp"
#include "astralix/modules/renderer/managers/scene-manager.hpp"
#include "astralix/shared/foundation/exceptions/base-exception.hpp"
#include "iostream"
#include "scenes/prologue.hpp"

int handleException(astralix::BaseException exception) {
  std::cout << exception.what() << std::endl;

  return -1;
}

#define INIT(runner)                                                           \
  auto app = astralix::runner::init();                                         \
                                                                               \
  std::filesystem::path dir =                                                  \
      std::filesystem::absolute(std::filesystem::path(__FILE__))               \
          .parent_path()                                                       \
          .parent_path() /                                                     \
      "src";                                                                   \
                                                                               \
  auto project = astralix::Project::create(                                    \
      {.name = "Sandbox",                                                      \
       .directory = dir,                                                       \
       .resources = {.directory = dir.parent_path().append("assets")}          \
                                                                               \
      });                                                                      \
                                                                               \
  astralix::ProjectManager::get()->add_project(project);                       \
                                                                               \
  astralix::SceneManager::get()->add_scene<Prologue>();                        \
                                                                               \
  app->start();                                                                \
  app->run();

int main(int argc, char **argv) {
  try {
#ifdef ASTRA_EDITOR
    INIT(Editor);
#else
    INIT(Application);
#endif
  } catch (astralix::BaseException exception) {
    return handleException(exception);
  }

  return 0;
}
