#include "scene.hpp"
#include "serializers/scene-serializer.hpp"

namespace astralix {
Scene::Scene(std::string name) : m_name(name), m_serializer() {}
void Scene::serialize() { return m_serializer->serialize(); }
void Scene::save() {}
void Scene::load() { m_serializer->deserialize(); }
} // namespace astralix
