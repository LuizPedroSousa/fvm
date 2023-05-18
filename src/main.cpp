#include "application.hpp"

int handleException(BaseException exception) {
  std::cout << exception.message << std::endl;

  return -1;
}

int main(int argc, char **argv) {

  auto canProcess = Application::init();

  if (canProcess.isLeft()) {
    return handleException(canProcess.left());
  }

  Application::get()->run();

  return 0;
}