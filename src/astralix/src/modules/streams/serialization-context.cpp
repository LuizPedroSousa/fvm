#include "serialization-context.hpp"
#include "adapters/json/json-serialization-context.hpp"
#include "assert.hpp"
#include "stream-buffer.hpp"

namespace astralix {

Ref<SerializationContext>
SerializationContext::create(SerializationFormat format) {
  switch (format) {
  case astralix::SerializationFormat::Json:
    return create_ref<JsonSerializationContext>();
  }

  ASTRA_EXCEPTION(true, "Serialization format not implemented");
}

Ref<SerializationContext>
SerializationContext::create(SerializationFormat format,
                             Scope<StreamBuffer> buffer) {
  switch (format) {
  case astralix::SerializationFormat::Json:
    return create_ref<JsonSerializationContext>(std::move(buffer));
  }

  ASTRA_EXCEPTION(true, "Serialization format not implemented");
}

} // namespace astralix
