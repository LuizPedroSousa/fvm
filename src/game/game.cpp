#include "game.hpp"
#include "engine.hpp"
#include "scenes/prologue.hpp"

Game::Game() {}

void Game::start() { astralix::SceneManager::get()->add_scene<Prologue>(); }