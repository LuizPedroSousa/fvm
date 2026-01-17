#pragma once

#include "arena.hpp"
#include "assert.hpp"
#include "guid.hpp"
#include "log.hpp"
#include "serialization-context.hpp"
#include "stream-buffer.hpp"

#include <json/value.h>
#include <stack>

namespace astralix {

class JsonSerializationContext : public SerializationContext {
public:
  JsonSerializationContext() { m_current.push(&m_root); }
  JsonSerializationContext(Scope<StreamBuffer> buffer);

  ~JsonSerializationContext() = default;
  void set_value(const SerializableValue &value) override;
  void set_value(Ref<SerializationContext> ctx) override;

  std::any get_root() override { return m_root; }

  size_t size() override { return m_root.size(); };
  ElasticArena::Block *to_buffer(ElasticArena &arena) override;

  std::string as_string() override { return m_current.top()->asString(); };
  int as_int() override { return m_current.top()->asInt(); };
  float as_float() override { return m_current.top()->asFloat(); };
  bool as_bool() override { return m_current.top()->asBool(); };

  ContextProxy operator[](const SerializableKey &key) override;

  std::string extension() const override { return ".json"; }
  void from_buffer(Scope<StreamBuffer> buffer) override;

  template <typename T> static std::string convert_key_to_string(const T &key) {
    if constexpr (std::is_same_v<T, int>) {
      return std::to_string(key); // int to string
    } else if constexpr (std::is_same_v<T, float>) {
      return std::to_string(key); // float to string
    } else if constexpr (std::is_same_v<T, std::string>) {
      return key; // already a string
    } else if constexpr (std::is_same_v<T, bool>) {
      return key ? "true" : "false"; // bool to "true" or "false"
    }

    ASTRA_EXCEPTION(true, "NO SUITABLE TYPE STRING CASTING FOR KEY");
  }

protected:
  JsonSerializationContext(Json::Value &root) : m_root(root) {
    m_current.push(&m_root);
  }
  Json::Value m_root;
  std::stack<Json::Value *> m_current;
};
} // namespace astralix
