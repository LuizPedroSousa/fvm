#include "application.hpp"

int handleException(BaseException exception) {
  std::cout << exception.what() << std::endl;

  return -1;
}

int main(int argc, char **argv) {

  try {
    auto canProcess = Application::init();

    if (canProcess.isLeft()) {
      return handleException(canProcess.left());
    }

    Application::get()->run();
  } catch (BaseException exception) {
    return handleException(exception);
  }

  return 0;
}