#pragma once
#include "either.hpp"
#include "exceptions/base-exception.hpp"
#include "game.hpp"

class Application {

  public:
  static Either<BaseException, Unit> init();
  void run();

  static Application *get() { return m_instance; }

  static float get_deltatime() { return get()->m_deltatime; }

  private:
  Application(){};
  ~Application();

  void on_update();

  Game m_game;

  float m_deltatime;
  float m_current_frame;
  float m_last_frame;

  static Application *m_instance;
};
