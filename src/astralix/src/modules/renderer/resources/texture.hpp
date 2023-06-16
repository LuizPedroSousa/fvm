#pragma once
#include "either.hpp"
#include "exceptions/base-exception.hpp"
#include "filesystem"
#include "resource.hpp"
#include "shader.hpp"
#include "string"
#include "tuple"

namespace astralix {

class Texture : public Resource {
public:
  Texture();

  static Either<BaseException, Texture>
  create(ResourceID resource_id, std::string name, const char *filename);

  static Either<BaseException, Texture>
  create_cubemap(ResourceID id, std::string name,
                 std::vector<std::string> faces);

  static Either<BaseException, Texture *> create_many(
      std::initializer_list<std::tuple<ResourceID, std::string, const char *>>
          textures);

  const u_int get_id() { return m_id; }

  const std::string get_name() { return m_name; }

private:
  unsigned int m_id;
  std::string m_name;
  int m_width;
  int m_height;

  Texture(RESOURCE_INIT_PARAMS, unsigned int p_id, std::string p_name,
          int p_width, int p_height);
  static std::filesystem::path get_path(const char *filename);

  static Either<BaseException, Unit> load_image(std::string filename,
                                                int *width, int *height,
                                                int *nr_channels,
                                                unsigned char **data);

  static int get_image_format(int nr_channels);
};
} // namespace astralix