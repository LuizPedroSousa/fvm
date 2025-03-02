#include "scene.hpp"
#include "serializers/scene-serializer.hpp"

namespace astralix {
Scene::Scene(std::string name) : m_name(name), m_serializer() {}
Json::Value Scene::serialize() { return m_serializer->serialize(); }
void Scene::save() { m_serializer->save(); }
void Scene::load() { m_serializer->deserialize(); }
} // namespace astralix
