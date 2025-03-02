#include "storage-buffer.hpp"

#include "engine.hpp"
#include "platform/OpenGL/opengl-storage-buffer.hpp"

namespace astralix {

Ref<StorageBuffer> StorageBuffer::create(uint32_t size) {
  return create_renderer_component_ref<StorageBuffer, OpenGLStorageBuffer>(
      Engine::get()->renderer_api->get_api(), size);
}

} // namespace astralix
