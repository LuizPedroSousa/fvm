#pragma once

#include "base-manager.hpp"
#include "base.hpp"
#include "entities/scene.hpp"
#include "vector"
#include "entities/serializers/scene-serializer.hpp"


namespace astralix {
  class SceneManager : public BaseManager<SceneManager> {
  public:
    template <typename T> void add_scene() {
      Ref<T> scene = create_ref<T>();

      scene->m_serializer = create_scope<SceneSerializer>(scene);

      m_scenes.push_back(std::move(scene));
    };

    Scene* get_active_scene() {
      if (m_scenes.size() > 0) {
        return m_scenes[0].get();
      }

      return nullptr;
    }

    ~SceneManager() {}
    SceneManager() {}

  private:
    std::vector<Ref<Scene>> m_scenes;
  };

} // namespace astralix