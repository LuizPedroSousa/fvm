#include "game.hpp"
#include "engine.hpp"
#include "scenes/prologue.hpp"

Game::Game() {}

void Game::start() {
  astralix::Engine::get()->get_entity_manager()->add_entity<Prologue>();
}