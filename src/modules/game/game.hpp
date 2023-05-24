#pragma once
#include "either.hpp"
#include "exceptions/base-exception.hpp"
#include "memory"
#include "scene.hpp"
#include "vector"

#include "managers/component-manager.hpp"
#include "managers/entity-manager.hpp"
#include "managers/system-manager.hpp"

class Game {
  public:
  Either<BaseException, Unit> start();

  void update();

  static Game *get() { return m_instance; }
  static void init();

  EntityManager *get_entity_manager() {
    return m_entity_manager;
  }

  SystemManager *get_system_manager() { return m_system_manager; }
  ComponentManager *get_component_manager() { return m_component_manager; }

  private:
  Game();

  SystemManager *m_system_manager;
  ComponentManager *m_component_manager;
  EntityManager *m_entity_manager;

  static Game *m_instance;
};