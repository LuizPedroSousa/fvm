#pragma once
#include "either.hpp"
#include "exceptions/base-exception.hpp"
#include "resources/resource.hpp"
#include "tuple"

class Shader : public Resource {
public:
  Shader();

  unsigned int signed_to = 0;
  unsigned int vertex = 0;
  unsigned int fragment = 0;

  const char *fragment_filename;
  const char *vertex_filename;

  Either<BaseException, Shader> static create(ResourceID id,
                                              const char *vertex_filename,
                                              const char *fragment_filename);

  Either<BaseException, Shader *> static create_many(
      std::tuple<ResourceID, const char *, const char *> files[], size_t size);

private:
  Shader(RESOURCE_INIT_PARAMS, u_int vertex, const char *vertex_filename,
         u_int fragment, const char *fragment_filename);

  static Either<BaseException, u_int> compile(const char *filename, int type);
  static const char *get_file(const char *filename);
};
