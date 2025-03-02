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

  Json::Value ModelComponentSerializer::serialize() {
    ComponentSerializer::serialize();

    Json::Value root = ComponentSerializer::serialize();

    return root;
  }

  void ModelComponentSerializer::save() {}

  void ModelComponentSerializer::deserialize() {}

} // namespace astralix
