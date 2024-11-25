#ifndef REND_BUFFER_H
#define REND_BUFFER_H

#include "rendmath.h"
#include "rendgl.h"

#include <sys/memory.h>
#include <sys/vector.h>

namespace rend
{

struct Buffer : sys::base
{
  Buffer();
  Buffer(const Buffer& _copy);
  Buffer& operator=(const Buffer& _assign);
  ~Buffer();

  void add(float _value);
  void add(vec2 _value);
  void add(vec3 _value);
  void add(vec4 _value);

  GLuint id();
  int components() const;
  size_t size() const;

private:
  sys::zero<int> m_type;
  sys::vector<float> m_data;

  sys::zero<bool> m_dirty;
  sys::zero<GLuint> m_id;
};

}

#endif

