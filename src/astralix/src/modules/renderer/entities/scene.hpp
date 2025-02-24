#pragma once
#include "ecs/managers/entity-manager.hpp"
#include "either.hpp"

#include "ecs/guid.hpp"
#include "exceptions/base-exception.hpp"
#include "serializers/scene-serializer.hpp"
#include "string"
#include <utility>

namespace astralix {

class SceneSerializer;

class Scene {
public:
  Scene(std::string name) : m_name{name} {};

  virtual void start() = 0;
  virtual void update() = 0;

  void save();
  void load();

  template <typename T, typename... Args> T *add_entity(Args &&...params) {
    auto manager = EntityManager::get();

    return manager->add_entity<T>(std::forward<Args>(params)...);
  }

  SceneID get_id() const { return m_id; }
  std::string get_name() const { return m_name; }

  ~Scene() {}

  friend class SceneManager;

protected:
  Scope<SceneSerializer> m_serializer;
  SceneID m_id;
  std::string m_name;
};

} // namespace astralix
