#include "serializers/project-serializer.hpp"
#include "context-proxy.hpp"
#include "project.hpp"
#include "serialization-context.hpp"
#include "serializer.hpp"

namespace astralix {

inline bool is_set(const std::string &v) { return !v.empty(); }
inline bool is_set(int v) { return v != 0; }
inline bool is_set(bool v) { return true; }

#define SET_CONFIG(value, key)                                                 \
  do {                                                                         \
    auto &&_v = (value);                                                       \
    if (is_set(_v))                                                            \
      config.key = _v;                                                         \
  } while (0)

ProjectSerializer::ProjectSerializer(Ref<Project> project,
                                     Ref<SerializationContext> ctx)
    : Serializer(std::move(ctx)), m_project(project) {}

ProjectSerializer::ProjectSerializer() {}

void ProjectSerializer::serialize() {
  SerializationContext &ctx = *m_ctx.get();

  auto &config = m_project->get_config();

  ctx["config"]["name"] = config.name;
  ctx["config"]["directory"] = config.directory;
  ctx["config"]["resources"]["directory"] = config.resources.directory;
}

void ProjectSerializer::deserialize() {
  auto &config = m_project->get_config();

  SerializationContext &ctx = *m_ctx.get();

  SET_CONFIG(ctx["name"].as<std::string>(), name);
  SET_CONFIG(ctx["directory"].as<std::string>(), directory);
  SET_CONFIG(ctx["resources"]["directory"].as<std::string>(),
             resources.directory);

  SET_CONFIG(config.serialization.formatFromString(
                 ctx["serialization"]["format"].as<std::string>()),
             serialization.format);

  auto size = ctx["windows"].size();

  for (int i = 0; i < size; i++) {
    auto current = ctx["windows"][i];

    WindowConfig window{
        .title = current["title"].as<std::string>(),
        .height = current["height"].as<int>(),
        .width = current["width"].as<int>(),
    };

    config.windows.push_back(window);
  }
}

} // namespace astralix
