#pragma once
#include "base.hpp"
#include "filesystem"
#include "resource.hpp"
#include "string"
#include "vector"

namespace astralix {

struct Image {
  int width;
  int height;
  int nr_channels;
  unsigned char *data;
};

class Texture : public Resource {
public:
  Texture(const ResourceID &resource_id) : Resource(resource_id) {};
  virtual void bind() const = 0;
  virtual void active(uint32_t slot) const = 0;
  virtual uint32_t get_renderer_id() const = 0;
  virtual uint32_t get_width() const = 0;
  virtual uint32_t get_height() const = 0;

protected:
  static std::filesystem::path get_path(const std::string &path);

  static Image load_image(std::string filename, bool flip_image_on_loading);
  static void free_image(u_char *data);
  static int get_image_format(int nr_channels);
};

class Texture2D : public Texture {
public:
  static Ref<Texture2D> create(const ResourceID &resource_id,
                               const std::string &path,
                               const bool flip_image_on_loading = false);

protected:
  Texture2D(const ResourceID &resource_id) : Texture(resource_id) {};
};

class Texture3D : public Texture {
public:
  static Ref<Texture3D> create(const ResourceID &resource_id,
                               const std::vector<std::string> &faces_path);

protected:
  Texture3D(const ResourceID &resource_id) : Texture(resource_id) {};
};

} // namespace astralix
