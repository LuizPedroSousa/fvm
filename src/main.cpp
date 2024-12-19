#include "astralix/src/modules/application/application.hpp"
#include "astralix/src/shared/exceptions/base-exception.hpp"
#include "game/game.hpp"

int handleException(astralix::BaseException exception) {
  std::cout << exception.what() << std::endl;

  return -1;
}

int main(int argc, char **argv) {
  try {
    Game game;
    astralix::Application *app = astralix::Application::init();

    game.start();
    app->start();

    app->run();
  } catch (astralix::BaseException exception) {
    return handleException(exception);
  }

  return 0;
}
