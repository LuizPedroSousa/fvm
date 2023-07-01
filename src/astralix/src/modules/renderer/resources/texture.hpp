#pragma once
#include "either.hpp"
#include "exceptions/base-exception.hpp"
#include "filesystem"
#include "resource.hpp"
#include "shader.hpp"
#include "string"
#include "vector"

namespace astralix {

struct CreateTextureDTO {
  ResourceID id;
  const char *filename;
  bool flip_image_on_loading;

  CreateTextureDTO(ResourceID id, const char *filename,
                   bool flip_image_on_loading = false)
      : id(id), filename(filename),
        flip_image_on_loading(flip_image_on_loading) {}
};

struct Image {
  int width;
  int height;
  int nr_channels;
  unsigned char *data;
};

class Texture : public Resource {
public:
  Texture(){};

  static Either<BaseException, Texture> create(CreateTextureDTO dto);

  static Either<BaseException, Texture>
  create_cubemap(ResourceID id, std::vector<std::string> faces);

  static Either<BaseException, Texture *>
  create_many(std::initializer_list<CreateTextureDTO> textures);

  const u_int get_id() { return m_id; }

private:
  unsigned int m_id;

  Texture(RESOURCE_INIT_PARAMS, unsigned int p_id);
  static std::filesystem::path get_path(const char *filename);

  static Either<BaseException, Image> load_image(std::string filename,
                                                 bool flip_image_on_loading);

  static int get_image_format(int nr_channels);
};
} // namespace astralix