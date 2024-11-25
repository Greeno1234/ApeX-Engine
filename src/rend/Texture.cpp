#include "Texture.h"
#include "Exception.h"
#include "Debug.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <sys/io.h>

namespace rend
{

Texture::Texture()
{
  size(256, 256);
}

Texture::Texture(const sys::string& _path)
{
  load(_path);
}

Texture::Texture(int _width, int _height)
{
  size(_width, _height);
}

Texture::Texture(const Texture& _copy)
  : m_data(_copy.m_data)
  , m_width(_copy.m_width)
  , m_height(_copy.m_height)
  , m_dirty(true)
{ }

Texture& Texture::operator=(const Texture& _assign)
{
  if(this == &_assign) return *this;

  m_data = _assign.m_data;
  m_width = _assign.m_width;
  m_height = _assign.m_height;
  m_dirty = true;

  return *this;
}

Texture::~Texture()
{
  if(m_id)
  {
    Debug::out("glDeleteTextures");
    glDeleteTextures(1, &m_id);
  }
}

void Texture::load(const sys::string& _path)
{
  int width = 0;
  int height = 0;

  stbi_set_flip_vertically_on_load(1);

  float *data = stbi_loadf(_path.unsafe_raw(),
    &width, &height, NULL, 4);

  stbi_set_flip_vertically_on_load(0);

  if(!data)
  {
    throw Exception("Failed to load texture [" + _path + "]");
  }

  size(width, height);

  for(size_t pi = 0; pi < width * height * 4; ++pi)
  {
    m_data[pi] = data[pi];
  }

  free(data);
  m_dirty = true;
}

void Texture::load(const sys::vector<unsigned char>& _data)
{
  int width = 0;
  int height = 0;
  int rgba = 0;

  stbi_set_flip_vertically_on_load(1);
  float *data = stbi_loadf_from_memory(&_data[0], _data.size(), &width, &height, NULL, 4);
  stbi_set_flip_vertically_on_load(0);

  if(!data)
  {
    throw Exception("Failed to load texture [unsigned char[]]");
  }

  size(width, height);

  for(size_t pi = 0; pi < width * height * 4; ++pi)
  {
    m_data[pi] = data[pi];
  }

  free(data);
  m_dirty = true;
}

void Texture::size(int _width, int _height)
{
  if(m_width == _width && m_height == _height)
  {
    return;
  }

  m_data.resize(_width * _height * 4);
  m_width = _width;
  m_height = _height;
  m_dirty = true;
}

vec2 Texture::size() const
{
  return vec2(m_width, m_height);
}

int Texture::width() const
{
  return m_width;
}

int Texture::height() const
{
  return m_height;
}

void Texture::pixel(int _x, int _y, vec4 _pixel)
{
  if(_x < 0 || _x >= m_width
    || _y < 0 || _y >= m_height)
  {
    throw Exception("Pixel outside texture bounds");
  }

  size_t offset = m_width * _y * 4 + _x * 4;

  m_data[offset] = _pixel.x;
  m_data[offset + 1] = _pixel.y;
  m_data[offset + 2] = _pixel.z;
  m_data[offset + 3] = _pixel.w;
}

vec4 Texture::pixel(int _x, int _y) const
{
  if(_x < 0 || _x >= m_width
    || _y < 0 || _y >= m_height)
  {
    throw Exception("Pixel outside texture bounds");
  }

  size_t offset = m_width * _y * 4 + _x * 4;

  vec4 rtn(m_data[offset],
    m_data[offset + 1],
    m_data[offset + 2],
    m_data[offset + 3]);

  return rtn;
}

GLuint Texture::id()
{
  if(!m_id)
  {
    Debug::out("glGenTextures");
    glGenTextures(1, &m_id);

    if(!m_id)
    {
      throw Exception("Failed to create texture");
    }
  }

  if(m_dirty)
  {
    glBindTexture(GL_TEXTURE_2D, m_id);

    Debug::out("glTexImage2D");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA,
      GL_FLOAT, &m_data[0]);

    Debug::out("glGenerateMipmap");
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    m_dirty = false;
  }

  return m_id;
}

}

