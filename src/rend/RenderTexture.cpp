#include "RenderTexture.h"
#include "Exception.h"
#include "Debug.h"

#include <sys/io.h>

namespace rend
{

RenderTexture::RenderTexture()
{
  size(256, 256);
}

RenderTexture::RenderTexture(const RenderTexture& _copy)
  : m_width(_copy.m_width)
  , m_height(_copy.m_height)
  , m_dirty(true)
{ }

RenderTexture& RenderTexture::operator=(const RenderTexture& _assign)
{
  if(this == &_assign) return *this;

  size(_assign.m_width, _assign.m_height);

  return *this;
}

RenderTexture::~RenderTexture()
{
  if(m_id)
  {
    Debug::out("Debug: glDeleteFramebuffers");
    glDeleteFramebuffers(1, &m_id);
  }

  if(m_textureid)
  {
    Debug::out("glDeleteTextures");
    glDeleteTextures(1, &m_textureid);
  }

  if(m_depthid)
  {
    Debug::out("glDeleteRenderbuffers");
    glDeleteRenderbuffers(1, &m_depthid);
  }
}

void RenderTexture::size(int _width, int _height)
{
  if(m_width == _width && m_height == _height)
  {
    return;
  }

  m_width = _width;
  m_height = _height;
  m_dirty = true;
}

int RenderTexture::width() const
{
  return m_width;
}

int RenderTexture::height() const
{
  return m_height;
}

void RenderTexture::clear()
{
  glBindFramebuffer(GL_FRAMEBUFFER, id());
  glClearColor(0.0f, 0.5f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint RenderTexture::texture_id()
{
  id();
  return m_textureid;
}

GLuint RenderTexture::id()
{
  if(!m_id)
  {
    Debug::out("glGenTextures");
    glGenTextures(1, &m_textureid);

    if(!m_textureid)
    {
      throw Exception("Failed to create texture");
    }

    glBindTexture(GL_TEXTURE_2D, m_textureid);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

//

    Debug::out("glGenRenderbuffers");
    glGenRenderbuffers(1, &m_depthid);

    if(!m_depthid)
    {
      throw Exception("Failed to create render buffer");
    }


//

    Debug::out("glGenFramebuffers");
    glGenFramebuffers(1, &m_id);

    if(!m_id)
    {
      throw Exception("Failed to create frame buffer");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, m_id);

    Debug::out("glFramebufferTexture2D");
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
      GL_TEXTURE_2D, m_textureid, 0);

    Debug::out("glFramebufferRenderbuffer");
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
      GL_RENDERBUFFER, m_depthid);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  if(m_dirty)
  {
    glBindTexture(GL_TEXTURE_2D, m_textureid);
    Debug::out("glTexImage2D");

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA,
      GL_UNSIGNED_BYTE, NULL);

    glBindTexture(GL_TEXTURE_2D, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, m_depthid);
    Debug::out("glRenderbufferStorage");
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    m_dirty = false;
  }

  return m_id;
}

}

