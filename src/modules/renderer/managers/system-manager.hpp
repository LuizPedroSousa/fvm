#pragma once

#include "utils/guid.hpp"

#include "memory"
#include "systems/base/isystem.hpp"
#include "unordered_map"
#include "vector"

class SystemManager {

  public:
  SystemManager();
  ~SystemManager();

  template <class T, class... Args>
  T *add_system(Args &&...params) {
    const SystemTypeID type_id = T::system_type_id();

    // avoid multiple registrations of the same system
    auto it = this->m_system_table.find(type_id);
    if ((this->m_system_table.find(type_id) != this->m_system_table.end()) && (it->second != nullptr))
      return reinterpret_cast<T *>(it->second.get());

    auto system = std::make_shared<T>(std::forward<Args>(params)...);
    system->m_system_manager = this;
    this->m_system_table[type_id] = system;

    // resize dependency table
    if (type_id + 1 > this->m_system_dependency_table.size()) {
      this->m_system_dependency_table.resize(type_id + 1);
      for (int i = 0; i < this->m_system_dependency_table.size(); ++i)
        this->m_system_dependency_table[i].resize(type_id + 1);
    }

    // add to work list
    this->m_system_work_order.push_back(system);

    return system.get();
  }

  template <class System_, class Dependency_>
  void add_system_dependency(System_ target, Dependency_ dependency) {
    const TypeID target_id = target->get_system_type_id();
    const TypeID dependency_id = dependency->get_system_type_id();

    if (this->m_system_dependency_table[target_id][dependency_id] != true) {
      this->m_system_dependency_table[target_id][dependency_id] = true;
      // LOG(Info(), "added " << dependency->get_system_type_name() << " as dependency to " << target->get_system_type_name());
    }
  }

  template <class Target_, class Dependency_, class... Dependencies>
  void add_system_dependency(Target_ target, Dependency_ dependency, Dependencies &&...dependencies) {
    const TypeID target_id = target->get_system_type_id();
    const TypeID dependency_id = dependency->get_system_type_id();

    if (this->m_system_dependency_table[target_id][dependency_id] != true) {
      this->m_system_dependency_table[target_id][dependency_id] = true;
      // LOG(Info(), "added " << dependency->get_system_type_name() << " as dependency to " << target->get_system_type_name());
    }

    this->add_system_dependency(target, std::forward<Dependencies>(dependencies)...);
  }

  void update_system_work_order();

  template <class T>
  inline T *get_system() const {
    auto it = this->m_system_table.find(T::system_type_id());

    return it != this->m_system_table.end() ? (T *)it->second.get() : nullptr;
  }

  template <class T>
  void enable_system() {
    const SystemTypeID type_id = T::system_type_id();

    // get system
    auto it = this->m_system_table.find(type_id);
    if (it != this->m_system_table.end()) {
      if (it->second->m_enabled == true)
        return;

      it->second->m_enabled = true;
    } else {
      //				throw new EXCEPTION_DEBUG_INFO();
      // LOG(Warning(), "Trying to enable system " << type_id << ", but system is not registered yet.");
    }
  }

  template <class T>
  void disable_system() {
    const SystemTypeID type_id = T::system_type_id();

    // get system
    auto it = this->m_system_table.find(type_id);
    if (it != this->m_system_table.end()) {
      if (it->second->m_enabled == false)
        return;

      it->second->m_enabled = false;
    } else {
      //				throw new EXCEPTION_DEBUG_INFO();
      // LOG(Warning(), "Trying to disable system " << type_id << ", but system is not registered yet.");
    }
  }

  template <class T>
  void set_system_update_interval(double interval_ms) {
    const SystemTypeID type_id = T::system_type_id();

    // get system
    auto it = this->m_system_table.find(type_id);
    if (it != this->m_system_table.end()) {
      it->second->m_updater_internal = interval_ms;
    } else {
      //				throw new EXCEPTION_DEBUG_INFO();
      // LOG(Warning(), "Trying to change system's " << type_id << " update interval, but system is not registered yet.");
    }
  }

  Either<BaseException, Unit> start();
  void fixed_update(double fixed_dt_ms);
  void update(double dt_ms);

  private:
  template <typename T>
  void depth_first_search(T vertex,
                          std::vector<int> &VERTEX_STATE,
                          const std::vector<std::vector<bool>> &EDGES,
                          std::vector<T> &output) {
    VERTEX_STATE[vertex] = 1;

    for (int i = 0; i < VERTEX_STATE.size(); ++i) {
      if (EDGES[i][vertex] == true && VERTEX_STATE[i] == 0)
        depth_first_search<T>(i, VERTEX_STATE, EDGES, output);
    }

    VERTEX_STATE[vertex] = 2;
    output.push_back(vertex);
  }

  using ISystem_ptr = std::shared_ptr<ISystem>;

  std::unordered_map<SystemTypeID, ISystem_ptr> m_system_table;

  // System Dependency Table
  //    ---   |	 Dependency SystemID  |
  // SystemID |   0   |   1   |   2   |
  //     0    | true  | false | false |
  //     1    | false | false | false |
  //     2    | false | true  | false |

  std::vector<std::vector<bool>> m_system_dependency_table;
  std::vector<ISystem_ptr> m_system_work_order;
};
