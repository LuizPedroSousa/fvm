#pragma once
#include "ecs/entities/entity.hpp"

#include "ecs/managers/component-manager.hpp"
#include "ecs/managers/entity-manager.hpp"
#include "ecs/managers/system-manager.hpp"
#include "managers/resource-manager.hpp"

namespace astralix {

class Engine {
public:
  static Engine *get() { return m_instance; }
  static void init();
  void end();

  void start();
  void update();

  EntityManager *get_entity_manager() { return m_entity_manager; }

  const IEntity *get_owner(const EntityID id) {
    return m_entity_manager->get_entity(id);
  }

  SystemManager *get_system_manager() { return m_system_manager; }
  ComponentManager *get_component_manager() { return m_component_manager; }
  ResourceManager *get_resource_manager() { return m_resource_manager; }

private:
  Engine();
  ~Engine();

  SystemManager *m_system_manager;
  ComponentManager *m_component_manager;
  EntityManager *m_entity_manager;
  ResourceManager *m_resource_manager;

  static Engine *m_instance;
};

}; // namespace astralix