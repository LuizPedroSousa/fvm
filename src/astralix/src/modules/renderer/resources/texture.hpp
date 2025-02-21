#pragma once
#include "base.hpp"
#include "ecs/guid.hpp"
#include "filesystem"
#include "resource.hpp"
#include "string"
#include "vector"
#include <optional>
#include <unordered_map>

namespace astralix {

struct Image {
  int width;
  int height;
  int nr_channels;
  unsigned char *data;
};

enum class TextureParameter {
  WrapS = 0,
  WrapT = 1,
  MagFilter = 2,
  MinFilter = 3
};

enum class TextureValue {
  Repeat = 0,
  ClampToEdge = 1,
  ClampToBorder = 2,
  Linear = 3,
  Nearest = 4,
  LinearMipMap = 5
};

enum class TextureFormat { Red = 0, RGB = 1, RGBA = 2 };

class Texture : public Resource {
public:
  Texture(const ResourceID &resource_id) : Resource(resource_id) {};
  virtual void bind() const = 0;
  virtual void active(uint32_t slot) const = 0;
  virtual uint32_t get_renderer_id() const = 0;
  virtual uint32_t get_width() const = 0;
  virtual uint32_t get_height() const = 0;

  int get_slot() { return m_slot; }
  void set_slot(int slot) { m_slot = slot; }

protected:
  static std::filesystem::path get_path(const std::string &path);

  static Image load_image(std::string filename, bool flip_image_on_loading);
  static void free_image(u_char *data);
  static int get_image_format(int nr_channels);
  int m_slot;
};

struct LoadImageConfig {
  std::string path;
  bool flip_image_on_loading = false;
};

struct TextureConfig {
  std::optional<LoadImageConfig> load_image;

  uint32_t width = 0;
  uint32_t height = 0;
  bool bitmap = true;
  TextureFormat format = TextureFormat::Red;

  std::unordered_map<TextureParameter, TextureValue> parameters;

  unsigned char *buffer = nullptr;
};

class Texture2D : public Texture {

public:
  static Ref<Texture2D>
  create(const ResourceID &resource_id, const std::string &path,
         const bool flip_image_on_loading = false,

         std::unordered_map<TextureParameter, TextureValue> parameters =
             {{TextureParameter::WrapS, TextureValue::Linear},
              {TextureParameter::WrapT, TextureValue::Linear},
              {TextureParameter::MagFilter, TextureValue::Nearest},
              {TextureParameter::MinFilter, TextureValue::Nearest}}

  );

  static Ref<Texture2D> create(const ResourceID &resource_id,
                               TextureConfig config);

protected:
  Texture2D(const ResourceID &resource_id) : Texture(resource_id) {};

  std::unordered_map<TextureParameter, TextureValue> parameters;
};

class Texture3D : public Texture {
public:
  static Ref<Texture3D> create(const ResourceID &resource_id,
                               const std::vector<std::string> &faces_path);

protected:
  Texture3D(const ResourceID &resource_id) : Texture(resource_id) {};
};

} // namespace astralix
