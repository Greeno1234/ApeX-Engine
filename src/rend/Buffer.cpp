#include "Buffer.h"
#include "Exception.h"
#include "Debug.h"

namespace rend
{

Buffer::Buffer()
{ }

Buffer::Buffer(const Buffer& _copy)
  : m_data(_copy.m_data)
  , m_type(_copy.m_type)
{
  if(m_data.size())
  {
    m_dirty = true;
  }
}

Buffer& Buffer::operator=(const Buffer& _assign)
{
  if(this == &_assign) return *this;

  if(m_data.size() || _assign.m_data.size())
  {
    m_dirty = true;
  }

  m_data = _assign.m_data;
  m_type = _assign.m_type;

  return *this;
}

Buffer::~Buffer()
{
  if(m_id)
  {
    Debug::out("glDeleteBuffers");
    glDeleteBuffers(1, &m_id);
  }
}

GLuint Buffer::id()
{
  if(!m_id)
  {
    Debug::out("glGenBuffers");
    glGenBuffers(1, &m_id);

    if(!m_id)
    {
      throw Exception("Failed to create vertex buffer");
    }
  }

  if(m_dirty)
  {
    glBindBuffer(GL_ARRAY_BUFFER, m_id);

    if(m_data.size())
    {
      Debug::out("glBufferData");
      glBufferData(GL_ARRAY_BUFFER, m_data.size() * sizeof(m_data[0]), &m_data[0], GL_STATIC_DRAW);
    }
    else
    {
      Debug::out("glBufferData");
      glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    m_dirty = false;
  }

  return m_id;
}

int Buffer::components() const
{
  return m_type;
}

size_t Buffer::size() const
{
  if(!m_type) return 0;
  return m_data.size() / m_type;
}

void Buffer::add(vec3 _value)
{
  if(!m_type)
  {
    m_type = 3;
  }

  if(m_type != 3)
  {
    throw Exception("Mixed components not supported");
  }

  m_data.push_back(_value.x);
  m_data.push_back(_value.y);
  m_data.push_back(_value.z);
  m_dirty = true;
}
void Buffer::add(vec2 _value)
{
  if(!m_type)
  {
    m_type = 2;
  }

  if(m_type != 2)
  {
    throw Exception("Mixed components not supported");
  }

  m_data.push_back(_value.x);
  m_data.push_back(_value.y);
  m_dirty = true;
}

}

