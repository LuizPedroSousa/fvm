#pragma once
#include "base.hpp"
#include "ecs/guid.hpp"
#include "glm/glm.hpp"
#include "resources/resource.hpp"
#include <map>

namespace astralix {
struct CharacterGlyph {
  ResourceID texture_id;
  glm::ivec2 size;
  glm::ivec2 bearing;
  unsigned int advance;
};

class Font : public Resource {

public:
  Font(const ResourceID &resource_id,
       std::map<char, CharacterGlyph> characters);
  static Ref<Font> create(const ResourceID &id, const char *font_path);

  std::map<char, CharacterGlyph> get_characters() const { return m_characters; }

private:
  std::map<char, CharacterGlyph> m_characters;
};
} // namespace astralix
