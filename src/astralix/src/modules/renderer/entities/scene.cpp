#include "scene.hpp"

namespace astralix {
void Scene::save() { m_serializer->serialize(); }
void Scene::load() { m_serializer->deserialize(); }
} // namespace astralix
