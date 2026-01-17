#include "path.hpp"
#include "guid.hpp"

namespace astralix {

Path::Path(std::string relative_path, BaseDirectory base_directory)
    : m_relative_path(relative_path), m_base_directory(base_directory),
      m_path_id(PathID()) {}

Ref<Path> Path::create(std::string relative_path,
                       BaseDirectory base_directory) {
  return create_ref<Path>(relative_path, base_directory);
}

} // namespace astralix
