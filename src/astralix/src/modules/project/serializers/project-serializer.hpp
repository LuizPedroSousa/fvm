#pragma once
#include "base.hpp"
#include "project.hpp"
#include "serializer.hpp"

namespace astralix {

  class Project;

  class ProjectSerializer : public Serializer {

  public:
    ProjectSerializer(Ref<Project> project);

    ProjectSerializer();

    void save() override;
    Json::Value serialize() override;
    void deserialize() override;

  private:
    Ref<Project> m_project = nullptr;
  };

} // namespace astralix
