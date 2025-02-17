#pragma once
#include "assert.hpp"
#include "base.hpp"
#include "ecs/guid.hpp"
#include "events/event-dispatcher.hpp"
#include "events/event.hpp"
#include "vector"
#include <iostream>

#include "unordered_map"
#include <algorithm>
namespace astralix {

enum SchedulerType {
  FRAME,      // Executes once per frame (classic update loop)
  TICK,       // Executes at a fixed timestep (good for physics updates)
  REALTIME,   // Executes based on real-world clock time
  INTERVAL,   // Executes every X milliseconds
  DEFERRED,   // Executes at the end of the frame
  IMMEDIATE,  // Executes instantly when scheduled
  BACKGROUND, // Runs at a lower priority, for non-essential tasks
  POST_FRAME, // Executes after rendering but before swap
  PRE_FRAME,  // Executes before the frame starts
  IDLE,       // Executes when there’s nothing else to do
};

struct Scheduler {
  SchedulerID id;
  SchedulerType type;
  Ref<Event> event;
};

class EventScheduler {
public:
  static void init();

  static EventScheduler *get();

  template <typename E, typename... Args>
  SchedulerID schedule(SchedulerType type, Args &&...args) {
    auto event = create_ref<E>(std::forward<Args>(args)...);

    auto it = m_type_schedulers.find(type);

    Guid scheduler_id;

    Scheduler scheduler = {
        .id = scheduler_id, .type = type, .event = std::move(event)};

    if (it != m_type_schedulers.end()) {

      auto emplaced_event = m_schedulers.emplace(scheduler_id, scheduler);

      ASTRA_ASSERT_THROW(!emplaced_event.second,
                         "Error creating new Event Scheduler!");

      it->second.push_back(scheduler_id);

      return scheduler_id;
    }

    auto emplaced_event = m_schedulers.emplace(scheduler_id, scheduler);

    ASTRA_ASSERT_THROW(!emplaced_event.second,
                       "Error creating new Event Scheduler!");

    std::vector<SchedulerID> type_schedulers = {scheduler_id};

    auto emplaced_scheduler_type =
        m_type_schedulers.emplace(type, type_schedulers);

    ASTRA_ASSERT_THROW(!emplaced_scheduler_type.second,
                       "Error creating new Event Scheduler!");

    return scheduler_id;
  }

  void bind(SchedulerType type) {
    auto type_scheduler = m_type_schedulers.find(type);

    if (type_scheduler == m_type_schedulers.end()) {
      return;
    }

    auto dispatcher = EventDispatcher::get();

    for (const auto &schedule_id : type_scheduler->second) {
      auto id_scheduler = m_schedulers.find(schedule_id);

      if (id_scheduler == m_schedulers.end()) {
        continue;
      }

      auto scheduler = id_scheduler->second;
      dispatcher->dispatch(scheduler.event.get());
    }
  }

  void destroy(SchedulerID scheduler_id) {
    auto scheduler = m_schedulers.find(scheduler_id);

    m_schedulers.erase(scheduler->first);

    auto scheduler_ids = m_type_schedulers.at(scheduler->second.type);

    auto it =
        std::find(scheduler_ids.begin(), scheduler_ids.end(), scheduler_id);

    if (it != scheduler_ids.end()) {
      scheduler_ids.erase(it);
    }
  }

  std::unordered_map<SchedulerID, Scheduler> m_schedulers;
  std::unordered_map<SchedulerType, std::vector<SchedulerID>> m_type_schedulers;

private:
  EventScheduler() = default;
  static EventScheduler *m_instance;
};
} // namespace astralix
