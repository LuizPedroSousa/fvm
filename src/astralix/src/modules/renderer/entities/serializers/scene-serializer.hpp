#pragma once
#include "base.hpp"
#include "entities/scene.hpp"
#include "serializer.hpp"

namespace astralix {
  class Scene;
  class SceneSerializer : public Serializer {

  public:
    SceneSerializer(Ref<Scene> scene);
    SceneSerializer();

    void serialize() override;
    Json::Value parse() override;
    void deserialize() override;

  private:
    Ref<Scene> m_scene = nullptr;
  };

} // namespace astralix
