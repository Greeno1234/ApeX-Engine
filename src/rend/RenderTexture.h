#include "rendgl.h"

#include <sys/memory.h>

namespace rend
{

struct RenderTexture : sys::base
{
  RenderTexture();
  RenderTexture(const RenderTexture& _copy);
  RenderTexture& operator=(const RenderTexture& _assign);
  ~RenderTexture();

  int width() const;
  int height() const;
  void size(int _width, int _height);

  GLuint id();
  GLuint texture_id();

  void clear();

private:
  sys::zero<int> m_width;
  sys::zero<int> m_height;

  sys::zero<bool> m_dirty;
  sys::zero<GLuint> m_id;
  sys::zero<GLuint> m_textureid;
  sys::zero<GLuint> m_depthid;

};

}
