#pragma once
#include "ecs/entities/ientity.hpp"
#include "events/event.hpp"

namespace astralix {
struct EntityCreatedEvent : public Event {
  EntityCreatedEvent(IEntity *entity) : entity(entity), Event() {}

  IEntity *entity;
  EVENT_TYPE(EntityCreated)
};
} // namespace astralix
