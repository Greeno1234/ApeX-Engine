#ifndef REND_FONT_H
#define REND_FONT_H

#include "rendmath.h"
#include "Texture.h"
#include "Buffer.h"

#include <sys/string.h>
#include <sys/vector.h>
#include <sys/memory.h>

namespace rend
{

struct Glyph : sys::base
{
  sys::zero<char> c;
  vec4 rect;
  vec4 coords;
  Buffer buffer;
};

struct Font : sys::base
{
  Font(const sys::string& _path);

  sys::ptr<Glyph> glyph(char _c);
  sys::ptr<Texture> texture();

  vec2 size(const sys::string& _text);

private:
  Texture m_texture;
  sys::vector<Glyph> m_glyphs;

  void generate_glyphs();
  bool dirty_column(int _x);

};

}

#endif
