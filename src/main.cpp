#ifdef ASTRA_EDITOR
#include "astralix/src/modules/editor/editor.hpp"
#else
#include "astralix/src/modules/application/application.hpp"
#endif
#include "astralix/src/shared/foundation/exceptions/base-exception.hpp"
#include "game/game.hpp"
#include "iostream"

int handleException(astralix::BaseException exception) {
  std::cout << exception.what() << std::endl;

  return -1;
}

#define INIT(runner)                                                           \
  auto app = astralix::runner::init();                                         \
  game.start();                                                                \
  app->start();                                                                \
  app->run();

int main(int argc, char **argv) {
  try {
    Game game;

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
