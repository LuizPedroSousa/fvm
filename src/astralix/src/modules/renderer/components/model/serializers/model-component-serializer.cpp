#include "components/model/serializers/model-component-serializer.hpp"
#include "components/model/model-component.hpp"
#include "filesystem"
#include "json/json.h"

namespace astralix {

  ModelComponentSerializer::ModelComponentSerializer(
    ModelComponent* model_component)
    : ComponentSerializer(model_component) {
  }

  ModelComponentSerializer::ModelComponentSerializer() {}

  Json::Value ModelComponentSerializer::parse() {
    ComponentSerializer::parse();

    Json::Value root = ComponentSerializer::parse();

    return root;
  }

  void ModelComponentSerializer::serialize() {}

  void ModelComponentSerializer::deserialize() {}

} // namespace astralix
