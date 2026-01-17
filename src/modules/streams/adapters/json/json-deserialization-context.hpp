

#pragma once

#include "deserialization-context.hpp"
#include "stream-buffer.hpp"

#include <json/value.h>
#include <stack>

namespace astralix {

class JsonDeserializationContext : public DeserializationContext {
public:
  JsonDeserializationContext() {}

  ~JsonDeserializationContext() override {}

  ContextProxy operator[](const std::string &key) override;
};
} // namespace astralix
