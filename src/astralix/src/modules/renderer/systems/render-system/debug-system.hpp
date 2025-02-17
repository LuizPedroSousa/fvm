#include "ecs/entities/entity.hpp"
#include "ecs/managers/entity-manager.hpp"
#include "ecs/systems/system.hpp"

namespace astralix {

class DebugDepth : public Entity<DebugDepth> {
public:
  DebugDepth(ENTITY_INIT_PARAMS);
  ~DebugDepth() = default;

  void start();

  void update();

  void on_enable() override {};
  void on_disable() override {};
};

class DebugNormal : public Entity<DebugNormal> {
public:
  DebugNormal(ENTITY_INIT_PARAMS);
  ~DebugNormal() = default;

  void start();

  void update();

  void on_enable() override {};
  void on_disable() override {};
};

class DebugSystem : public System<DebugSystem> {
public:
  DebugSystem();
  ~DebugSystem();

  void start() override;
  void fixed_update(double fixed_dt) override;
  void pre_update(double dt) override;
  void update(double dt) override;

  EntityManager *m_entity_manager;
};

} // namespace astralix
