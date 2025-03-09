#include "serializers/project-serializer.hpp"
#include "log.hpp"
#include "serialization-context.hpp"
#include "serializer.hpp"

namespace astralix {

  ProjectSerializer::ProjectSerializer(Ref<Project> project,
    Ref<SerializationContext> ctx)
    : Serializer(std::move(ctx)), m_project(project) {
  }

  ProjectSerializer::ProjectSerializer() {}

  void ProjectSerializer::serialize() {
    SerializationContext& ctx = *m_ctx.get();

    auto& config = m_project->get_config();

    ctx["config"]["name"] = config.name;
    ctx["config"]["directory"] = config.directory;
    ctx["config"]["resources"]["directory"] = config.resources.directory;
  }

  void ProjectSerializer::deserialize() {
    auto& config = m_project->get_config();

    SerializationContext& ctx = *m_ctx.get();

    config.name = ctx["config"]["name"].as<std::string>();
    config.directory = ctx["config"]["directory"].as<std::string>();
    config.resources.directory =
      ctx["config"]["resources"]["directory"].as<std::string>();
  }

} // namespace astralix
