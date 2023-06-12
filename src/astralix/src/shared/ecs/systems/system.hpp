#pragma once

#include "ecs/guid.hpp"

#include "isystem.hpp"

#include "ecs/managers/system-manager.hpp"

#undef max
#undef min

namespace astralix {
template <class T> class System : public ISystem {

protected:
  friend class SystemManager;
  System() : m_system_manager(nullptr) {}

private:
  SystemManager *m_system_manager;

public:
  static const SystemTypeID SYSTEM_TYPE_ID;

  virtual ~System() {}

  virtual inline SystemTypeID get_system_type_id() const override {
    return SYSTEM_TYPE_ID;
  }

  static SystemTypeID system_type_id() { return SYSTEM_TYPE_ID; }

  virtual inline const char *get_system_type_name() const override {
    static const char *SYSTEM_TYPE_NAME{typeid(T).name()};
    return SYSTEM_TYPE_NAME;
  }

  template <class... Dependencies>
  void add_dependencies(Dependencies &&...dependencies) {
    this->m_system_manager->add_system_dependency(
        this, std::forward<Dependencies>(dependencies)...);
  }

  virtual void start() override = 0;
  virtual void fixed_update(double fixed_dt) override = 0;
  virtual void pre_update(double dt) override = 0;
  virtual void update(double dt) override = 0;
  virtual void post_update(double dt) override = 0;
};

template <typename T>
const SystemTypeID System<T>::SYSTEM_TYPE_ID = FamilyTypeID<ISystem>::get<T>();

} // namespace astralix
