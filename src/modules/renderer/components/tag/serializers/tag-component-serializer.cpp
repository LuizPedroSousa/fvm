#include "components/tag/serializers/tag-component-serializer.hpp"
#include "components/tag/tag-component.hpp"

namespace astralix {

TagComponentSerializer::TagComponentSerializer(
    TagComponent *transform_component)
    : ComponentSerializer(transform_component) {}

TagComponentSerializer::TagComponentSerializer() {}

void TagComponentSerializer::serialize() {
  ComponentSerializer::serialize();

  auto component = (TagComponent *)m_component;

  SerializationContext &ctx = *m_ctx.get();

  for (int i = 0; i < component->tags.size(); i++) {
    auto tag = component->tags.at(i);

    ctx[i]["name"] = tag;
  }
}

void TagComponentSerializer::deserialize() {}

} // namespace astralix
