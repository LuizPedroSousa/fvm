#pragma once
#include "ecs/entities/entity.hpp"
#include "either.hpp"

#include "exceptions/base-exception.hpp"

namespace astralix {

class Scene : public Entity<Scene> {
public:
  Scene(ENTITY_INIT_PARAMS) : ENTITY_INIT(){};
  Scene() {}

  virtual void start() = 0;
  virtual void update() = 0;

  ~Scene() {}
};

} // namespace astralix