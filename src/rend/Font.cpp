#include "Font.h"
#include "Exception.h"

namespace rend
{

Font::Font(const sys::string& _path)
  : m_texture(_path)
{
  generate_glyphs();
}

void Font::generate_glyphs()
{
  sys::string characters;
  characters += "!\"#$%&'()*+,-./";
  characters += "0123456789";
  characters += ":;<=>?@";
  characters += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  characters += "[\\]^_`";
  characters += "abcdefghijklmnopqrstuvwxyz";
  characters += "{|}~";

  int x = 0;

  for(size_t i = 0; i < characters.length(); ++i)
  {
    Glyph g;
    g.c = characters[i];
    g.rect.y = 0;
    g.rect.w = m_texture.height();

    while(1)
    {
      if(x >= m_texture.width())
      {
        throw Exception("End of image before all glyphs assigned");
      }

      if(dirty_column(x) == true)
      {
        g.rect.x = x;

        break;
      }

      x++;
    }

    while(1)
    {
      if(x >= m_texture.width())
      {
        throw Exception("Glyph bounds went past end of image");
      }

      if(dirty_column(x) == 0)
      {
        g.rect.z = x - g.rect.x;

        break;
      }

      x++;
    }

    g.coords.x = (float)g.rect.x / (float)m_texture.width();
    g.coords.y = 0;
    g.coords.z = (float)(g.rect.x + g.rect.z) / (float)m_texture.width();
    g.coords.w = 1;

    m_glyphs.push_back(g);

    if(g.c == '-')
    {
      g.c = ' ';
      g.rect.w = 1;
      m_glyphs.push_back(g);
    }
  }
}

sys::ptr<Glyph> Font::glyph(char _c)
{
  for(sys::vector<Glyph>::iterator it = m_glyphs.begin(); it != m_glyphs.end(); ++it)
  {
    if(it->c != _c) continue;

    if(!it->buffer.size())
    {
      it->buffer.add(rend::vec2(it->coords.x, it->coords.w));
      it->buffer.add(rend::vec2(it->coords.x, it->coords.y));
      it->buffer.add(rend::vec2(it->coords.z, it->coords.y));
      it->buffer.add(rend::vec2(it->coords.z, it->coords.y));
      it->buffer.add(rend::vec2(it->coords.z, it->coords.w));
      it->buffer.add(rend::vec2(it->coords.x, it->coords.w));
    }

    return &*it;
  }

  throw Exception("Invalid character specified");
}

bool Font::dirty_column(int _x)
{
  for(int y = 0; y < m_texture.height(); ++y)
  {
    vec4 col = m_texture.pixel(_x, y);

    if(col.w != 0) return true;
  }

  return false;
}

sys::ptr<Texture> Font::texture()
{
  return &m_texture;
}

vec2 Font::size(const sys::string& _text)
{
  vec2 rtn(0, m_texture.height());

  for(size_t ci = 0; ci < _text.length(); ++ci)
  {
    sys::ptr<Glyph> g = glyph(_text[ci]);

    rtn.x += g->rect.z + 1.0f;
  }

  return rtn;
}

}
