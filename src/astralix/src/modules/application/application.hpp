#pragma once

namespace astralix {
class Application {

public:
  static Application *init();
  void start();
  void run();

  static Application *get() { return m_instance; }
  static void end();

private:
  Application() {};
  ~Application();

  static Application *m_instance;
};

} // namespace astralix
