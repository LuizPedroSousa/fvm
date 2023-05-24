#include "system-manager.hpp"
#include "algorithm"
#include "systems/base/isystem.hpp"
#include "map"

SystemManager::SystemManager() {
}

SystemManager::~SystemManager() {
}

Either<BaseException, Unit> SystemManager::start() {
  for (ISystem_ptr system : m_system_work_order) {
    if (system->m_enabled) {
      auto hasStarted = system->start();
      ASSERT_COMPARE(hasStarted);
    }
  }

  return Unit();
}

void SystemManager::fixed_update(const double fixed_dt_ms) {
  for (ISystem_ptr system : m_system_work_order) {
    // increase interval since last update
    system->m_time_since_last_update = fixed_dt_ms;

    if (system->m_enabled) {
      system->fixed_update(fixed_dt_ms);
    }
  }
}

/**
 * \brief
 * \param dt_ms
 */
void SystemManager::update(const double dt_ms) {
  for (ISystem_ptr system : this->m_system_work_order) {
    // increase interval since last update
    system->m_time_since_last_update = dt_ms;

    // check systems update state
    system->m_needs_update = (system->m_updater_internal < 0.0f) || ((system->m_updater_internal > 0.0f) && (system->m_time_since_last_update > system->m_updater_internal));

    if (system->m_enabled && system->m_needs_update) {
      system->pre_update(dt_ms);
    }
  }

  for (ISystem_ptr system : this->m_system_work_order) {
    if (system->m_enabled && system->m_needs_update) {
      system->update(dt_ms);
      system->m_time_since_last_update = 0.0f;
    }
  }

  for (ISystem_ptr system : this->m_system_work_order) {
    if (system->m_enabled && system->m_needs_update) {
      system->post_update(dt_ms);
    }
  }
}

void SystemManager::update_system_work_order() {
  const size_t system_count = this->m_system_dependency_table.size();

  // create index array
  std::vector<int> indices(system_count);
  for (size_t i = 0; i < system_count; ++i)
    indices[i] = static_cast<int>(i);

  // determine vertex-groups
  std::vector<std::vector<SystemTypeID>> vertex_groups;
  std::vector<SystemPriority> group_priority;

  while (!indices.empty()) {
    SystemTypeID index = indices.back();
    indices.pop_back();

    if (index == -1)
      continue;

    std::vector<SystemTypeID> group;
    std::vector<SystemTypeID> member;

    SystemPriority current_group_priority = LOWEST_SYSTEM_PRIORITY;
    member.push_back(index);

    while (!member.empty()) {
      index = member.back();
      member.pop_back();

      for (int i = 0; i < static_cast<int>(indices.size()); ++i) {
        if (indices[i] != -1 && (this->m_system_dependency_table[i][index] == true || this->m_system_dependency_table[index][i] == true)) {
          member.push_back(i);
          indices[i] = -1;
        }
      }

      group.push_back(index);

      const ISystem_ptr sys = this->m_system_table[index];
      current_group_priority = std::max((sys != nullptr ? sys->m_priority : NORMAL_SYSTEM_PRIORITY), current_group_priority);
    }

    vertex_groups.push_back(group);
    group_priority.push_back(current_group_priority);
  }

  const size_t vertex_group_count = vertex_groups.size();

  // do a topological sort on groups w.r.t. groups priority!
  std::vector<int> vertex_states(system_count, 0);

  std::multimap<SystemPriority, std::vector<SystemTypeID>> sorted_vertex_groups;

  for (size_t i = 0; i < vertex_group_count; ++i) {
    auto g = vertex_groups[i];

    std::vector<SystemTypeID> order;

    for (size_t j = 0; j < g.size(); ++j) {
      if (vertex_states[g[j]] == 0)

        depth_first_search<SystemTypeID>(g[j], vertex_states, this->m_system_dependency_table, order);
    }

    std::reverse(order.begin(), order.end());

    sorted_vertex_groups.insert(std::pair<SystemPriority, std::vector<SystemTypeID>>(std::numeric_limits<SystemPriority>::max() - group_priority[i], order));
  }

  // re-build system work order
  this->m_system_work_order.clear();
  for (auto group : sorted_vertex_groups) {
    for (auto m : group.second) {
      const ISystem_ptr sys = this->m_system_table[m];
      if (sys != nullptr) {
        this->m_system_work_order.push_back(sys);
      }
    }
  }
}
