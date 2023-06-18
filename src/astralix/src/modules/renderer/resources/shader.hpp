#pragma once
#include "either.hpp"
#include "exceptions/base-exception.hpp"
#include "resources/resource.hpp"
#include "tuple"

namespace astralix {

class Shader : public Resource {
public:
  Shader();

  unsigned int signed_to = 0;
  unsigned int vertex    = 0;
  unsigned int fragment  = 0;
  unsigned int geometry  = 0;

  Either<BaseException, Shader> static create(
      ResourceID id, const char *vertex_filename, const char *fragment_filename,
      const char *geometry_filename = NULL);

  Either<BaseException, Shader *> static create_many(
      std::tuple<ResourceID, const char *, const char *, const char *> files[],
      size_t size);

private:
  Shader(RESOURCE_INIT_PARAMS, unsigned int vertex, const char *vertex_filename,
         unsigned int fragment, const char *fragment_filename);

  Shader(RESOURCE_INIT_PARAMS, unsigned int vertex, const char *vertex_filename,
         unsigned int fragment, const char *fragment_filename,
         unsigned int geometry, const char *geometry_filename);

  static Either<BaseException, unsigned int> compile(const char *filename,
                                                     int type);
  static const char *get_file(const char *filename);

  const char *m_fragment_filename;
  const char *m_geometry_filename;
  const char *m_vertex_filename;
};

} // namespace astralix