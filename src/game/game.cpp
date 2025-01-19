#include "game.hpp"
#include "managers/project-manager.hpp"
#include "managers/scene-manager.hpp"
#include "project.hpp"
#include "scenes/prologue.hpp"

Game::Game() {}

void Game::start() {

  auto dir = std::filesystem::current_path().parent_path().append("game");

  astralix::ProjectManager::get()->add_project(astralix::Project::create(
      {.name = "Game",
       .directory = dir,
       .resources = {.directory = dir.append("resources")}

      }));

  astralix::SceneManager::get()->add_scene<Prologue>();
}
