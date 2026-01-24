#pragma once
#include "base.hpp"
#include "serialization-context.hpp"

namespace astralix {

class Serializer {
public:
  virtual void serialize() = 0;
  virtual void deserialize() = 0;

  Ref<SerializationContext> get_ctx() { return m_ctx; }

  Serializer(Ref<SerializationContext> ctx) : m_ctx(ctx) {};
  Serializer();

protected:
  Ref<SerializationContext> m_ctx = nullptr;
};

} // namespace astralix
