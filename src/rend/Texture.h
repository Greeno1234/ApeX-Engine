#ifndef REND_TEXTURE_H
#define REND_TEXTURE_H

#include "rendmath.h"
#include "rendgl.h"

#include <sys/string.h>
#include <sys/vector.h>
#include <sys/memory.h>

namespace rend
{

struct Texture : sys::base
{
  Texture();
  Texture(const Texture& _copy);
  Texture& operator=(const Texture& _assign);
  ~Texture();

  Texture(const sys::string& _path);
  Texture(int _width, int _height);

  void load(const sys::string& _path);
  void load(const sys::vector<unsigned char>& _data);

  int width() const;
  void width(int _width);

  int height() const;
  void height(int _height);

  void size(int _width, int _height);
  vec2 size() const;

  vec4 pixel(int _x, int _y) const;
  void pixel(int _x, int _y, vec4 _pixel);

  GLuint id();

private:
  sys::vector<float> m_data;
  sys::zero<int> m_width;
  sys::zero<int> m_height;

  sys::zero<bool> m_dirty;
  sys::zero<GLuint> m_id;

};

}

#endif
