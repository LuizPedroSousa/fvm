#include "game.hpp"
#include "scenes/prologue.hpp"

Game::Game() {
}

void Game::start() {
  m_scenes.push_back(std::make_unique<Prologue>(Prologue()));

  m_scenes[m_current_scene]->start();
}

void Game::update() {
  m_scenes[m_current_scene]->update();
}