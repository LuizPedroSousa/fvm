#include "components/model/serializers/model-component-serializer.hpp"
#include "components/model/model-component.hpp"
#include "filesystem"
#include "json/json.h"

namespace astralix {

ModelComponentSerializer::ModelComponentSerializer(
    ModelComponent *model_component)
    : ComponentSerializer(model_component) {}

ModelComponentSerializer::ModelComponentSerializer() {}

void ModelComponentSerializer::serialize() { ComponentSerializer::serialize(); }

void ModelComponentSerializer::deserialize() {}

} // namespace astralix
