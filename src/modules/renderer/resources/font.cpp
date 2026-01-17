#include "font.hpp"
#include "assert.hpp"
#include "guid.hpp"
#include "managers/project-manager.hpp"
#include "managers/resource-manager.hpp"
#include "resources/texture.hpp"
#include <ft2build.h>
#include <string>
#include FT_FREETYPE_H
#include "glad/glad.h"

#include <iostream>

namespace astralix {

  Font::Font(const ResourceID& resource_id,
    std::map<char, CharacterGlyph> characters)
    : Resource(resource_id), m_characters(characters) {

  };

  Ref<Font> Font::create(const ResourceID& id, const char* font_path) {
    FT_Library ft;

    ASTRA_EXCEPTION(FT_Init_FreeType(&ft),
      "ERROR::FREETYPE: Could not init FreeType Library");

    FT_Face face;

    auto project = ProjectManager::get()->get_active_project();

    auto base_path =
      std::filesystem::path(project->get_config().resources.directory)
      .append(font_path);

    ASTRA_EXCEPTION(FT_New_Face(ft, base_path.c_str(), 0, &face),
      "ERROR::FREETYPE: Failed to load font");

    FT_Set_Pixel_Sizes(face, 0, 48);

    ASTRA_EXCEPTION(FT_Load_Char(face, 'X', FT_LOAD_RENDER),
      "ERROR::FREETYTPE: Failed to load Glyph");

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    std::map<char, CharacterGlyph> characters;

    auto resource_manager = ResourceManager::get();

    for (unsigned char c = 0; c < 128; c++) {
      // load character glyph
      if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        continue;
      }

      auto config = TextureConfig{
          .width = face->glyph->bitmap.width,
          .height = face->glyph->bitmap.rows,
          .bitmap = false,
          .format = TextureFormat::Red,
          .parameters = {{TextureParameter::WrapS, TextureValue::ClampToBorder},
                         {TextureParameter::WrapT, TextureValue::ClampToBorder},
                         {TextureParameter::MagFilter, TextureValue::Linear},
                         {TextureParameter::MinFilter, TextureValue::Linear}},
          .buffer = face->glyph->bitmap.buffer,

      };

      auto texture_id =
        id + std::string("glyph[") + std::to_string(c) + std::string("]");

      auto texture =
        resource_manager->load_texture(Texture2D::create(texture_id, config));

      CharacterGlyph character = {
          .texture_id = texture->get_resource_id(),
          .size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
          .bearing =
              glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
          .advance = static_cast<unsigned int>(face->glyph->advance.x) };

      characters.insert(std::pair<char, CharacterGlyph>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return create_ref<Font>(id, characters);
  }

} // namespace astralix
