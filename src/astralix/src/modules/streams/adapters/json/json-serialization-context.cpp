#include "adapters/json/json-serialization-context.hpp"
#include "arena.hpp"
#include "assert.hpp"
#include "base.hpp"
#include "context-proxy.hpp"
#include "guid.hpp"
#include "log.hpp"
#include "stream-buffer.hpp"
#include <any>
#include <json/forwards.h>
#include <json/reader.h>
#include <json/value.h>
#include <json/writer.h>
#include <string>
#include <variant>

namespace astralix {

ContextProxy

JsonSerializationContext::operator[](const SerializableKey &key) {
  Json::Value &current_node = *m_current.top();

  auto parsed_key = std::visit(
      [](auto &&key) -> std::variant<int, std::string> {
        using T = std::decay_t<decltype(key)>;

        if constexpr (std::is_same_v<T, int>) {
          return key; // Keep int as int
        } else {
          return convert_key_to_string(key); // Convert non-int keys to string
        }
      },
      key);

  auto sub_ctx = create_scope<JsonSerializationContext>();

  if (std::holds_alternative<std::string>(parsed_key)) {
    auto str_key = std::get<std::string>(parsed_key);

    sub_ctx->m_current.push(&current_node[str_key]);

    return ContextProxy(std::move(sub_ctx), str_key);
  }

  auto int_key = std::get<int>(parsed_key);

  sub_ctx->m_current.push(&current_node[int_key]);

  return ContextProxy(std::move(sub_ctx), std::to_string(int_key));
};

void JsonSerializationContext::set_value(const SerializableValue &value) {
  Json::Value *current_value = m_current.top();

  if (std::holds_alternative<int>(value)) {
    *current_value = std::get<int>(value);
  } else if (std::holds_alternative<float>(value)) {
    *current_value = std::get<float>(value);
  } else if (std::holds_alternative<std::string>(value)) {
    *current_value = std::get<std::string>(value);
  } else if (std::holds_alternative<bool>(value)) {
    *current_value = std::get<bool>(value);
  }
}

void JsonSerializationContext::set_value(Ref<SerializationContext> ctx) {
  Json::Value *current_value = m_current.top();

  auto j_ctx = static_cast<JsonSerializationContext *>(ctx.get());

  *current_value = j_ctx->m_root;
}

ElasticArena::Block *JsonSerializationContext::to_buffer(ElasticArena &arena) {
  Json::StreamWriterBuilder writer;
  std::string json_string = Json::writeString(writer, m_root);

  auto block = arena.allocate(json_string.size());

  std::memcpy(block->data, json_string.data(), json_string.size());

  return block;
};

JsonSerializationContext::JsonSerializationContext(Scope<StreamBuffer> buffer) {
  from_buffer(std::move(buffer));
};

void JsonSerializationContext::from_buffer(Scope<StreamBuffer> buffer) {
  Json::Reader reader;

  ASTRA_EXCEPTION(!reader.parse(buffer->data(), m_root, false),
                  "Failed to parse json");

  m_current = {};
  m_current.push(&m_root);
};

} // namespace astralix
