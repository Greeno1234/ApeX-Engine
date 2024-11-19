#include "Shader.h"
#include "Exception.h"
#include "Buffer.h"
#include "Texture.h"
#include "RenderTexture.h"
#include "Debug.h"

#include <sys/io.h>

namespace rend
{

enum UniformType
{
  E_TEXTURE = 0,
  E_INT,
  E_VEC2,
  E_VEC3,
  E_VEC4,
  E_MAT4
};

struct Shader::Uniform : sys::base
{
  sys::string name;
  sys::zero<int> type;
  sys::zero<GLint> location;

  sys::zero<int> int_value;
  sys::zero<float> float_value;
  vec2 vec2_value;
  vec3 vec3_value;
  vec4 vec4_value;
  mat4 mat4_value;
  sys::ptr<Texture> texture_value;
  sys::ptr<RenderTexture> rendertexture_value;
};

struct Shader::Attribute
{
  sys::string name;
  sys::zero<GLint> location;
  sys::ptr<Buffer> buffer;
};

Shader::Shader()
  : m_dirty(true)
  , m_blend(true)
{
  load(TEXTURE_SHADER);
}

Shader::Shader(const Shader& _copy)
  : m_source(_copy.m_source)
  , m_attributes(_copy.m_attributes)
  , m_uniforms(_copy.m_uniforms)
  , m_blend(_copy.m_blend)
  , m_dirty(true)
{ }

Shader& Shader::operator=(const Shader& _assign)
{
  if(this == &_assign) return *this;

  m_source = _assign.m_source;
  m_attributes = _assign.m_attributes;
  m_uniforms = _assign.m_uniforms;
  m_blend = _assign.m_blend;
  m_dirty = true;

  return *this;
}

Shader::~Shader()
{
  if(m_id)
  {
    Debug::out("glDeleteProgram");
    glDeleteProgram(m_id);
  }
}

Shader::Shader(int _type)
  : m_dirty(true)
  , m_blend(true)
{
  load(_type);
}

void Shader::load(const sys::string& _vertsrc, const sys::string& _fragsrc)
{
  m_source
    = "\n#ifdef VERTEX\n"
    + _vertsrc
    + "\n#endif\n"
    + "\n#ifdef FRAGMENT\n"
    + _fragsrc
    + "\n#endif\n";

  m_dirty = true;
}

void Shader::load(int _type)
{
  m_blend = true;
  m_depthtest = false;
  m_backfacecull = false;

  if(_type == TEXTURE_SHADER)
  {
    m_source =
      "\n#ifdef VERTEX\n                                    " \
      "attribute vec4 a_Position;                           " \
      "attribute vec2 a_TexCoord;                           " \
      "                                                     " \
      "uniform mat4 u_Projection;                           " \
      "uniform mat4 u_Model;                                " \
      "                                                     " \
      "varying vec2 v_TexCoord;                             " \
      "                                                     " \
      "void main()                                          " \
      "{                                                    " \
      "  gl_Position = u_Projection * u_Model * a_Position; " \
      "  v_TexCoord = a_TexCoord;                           " \
      "}                                                    " \
      "\n#endif\n                                           " \
      "\n#ifdef FRAGMENT\n                                  " \
      "varying vec2 v_TexCoord;                             " \
      "                                                     " \
      "uniform sampler2D u_Texture;                         " \
      "                                                     " \
      "void main()                                          " \
      "{                                                    " \
      "  vec4 tex = texture2D(u_Texture, v_TexCoord);       " \
      "  gl_FragColor = tex;                                " \
      "}                                                    " \
      "\n#endif\n                                           ";
  }
  else if(_type == TEXTURE_LIGHT_SHADER)
  {
    m_source =
      "\n#ifdef VERTEX\n                                    " \
      "attribute vec4 a_Position;                           " \
      "attribute vec2 a_TexCoord;                           " \
      "                                                     " \
      "uniform mat4 u_Projection;                           " \
      "uniform mat4 u_Model;                                " \
      "                                                     " \
      "varying vec2 v_TexCoord;                             " \
      "                                                     " \
      "void main()                                          " \
      "{                                                    " \
      "  gl_Position = u_Projection * u_Model * a_Position; " \
      "  v_TexCoord = a_TexCoord;                           " \
      "}                                                    " \
      "\n#endif\n                                           " \
      "\n#ifdef FRAGMENT\n                                  " \
      "varying vec2 v_TexCoord;                             " \
      "                                                     " \
      "uniform sampler2D u_Texture;                         " \
      "                                                     " \
      "void main()                                          " \
      "{                                                    " \
      "  vec4 tex = texture2D(u_Texture, v_TexCoord);       " \
      "  gl_FragColor = tex;                                " \
      "}                                                    " \
      "\n#endif\n                                           ";
  }
  else if(_type == COLOR_SHADER)
  {
    m_source =
      "\n#ifdef VERTEX\n                                    " \
      "attribute vec4 a_Position;                           " \
      "                                                     " \
      "uniform mat4 u_Projection;                           " \
      "uniform mat4 u_Model;                                " \
      "                                                     " \
      "void main()                                          " \
      "{                                                    " \
      "  gl_Position = u_Projection * u_Model * a_Position; " \
      "}                                                    " \
      "\n#endif\n                                           " \
      "\n#ifdef FRAGMENT\n                                  " \
      "uniform vec4 u_Color;                                " \
      "                                                     " \
      "void main()                                          " \
      "{                                                    " \
      "  vec4 tex = u_Color;                                " \
      "  gl_FragColor = tex;                                " \
      "}                                                    " \
      "\n#endif\n                                           ";
  }
  else if(_type == COLOR_LIGHT_SHADER)
  {
    m_source =
      "\n#ifdef VERTEX\n                                    " \
      "attribute vec4 a_Position;                           " \
      "                                                     " \
      "uniform mat4 u_Projection;                           " \
      "uniform mat4 u_Model;                                " \
      "                                                     " \
      "void main()                                          " \
      "{                                                    " \
      "  gl_Position = u_Projection * u_Model * a_Position; " \
      "}                                                    " \
      "\n#endif\n                                           " \
      "\n#ifdef FRAGMENT\n                                  " \
      "uniform vec4 u_Color;                                " \
      "                                                     " \
      "void main()                                          " \
      "{                                                    " \
      "  vec4 tex = u_Color;                                " \
      "  gl_FragColor = tex;                                " \
      "}                                                    " \
      "\n#endif\n                                           ";
  }
  else if(_type == GUI_SHADER)
  {
    m_source =
      "\n#ifdef VERTEX\n                                    " \
      "attribute vec2 a_Position;                           " \
      "attribute vec2 a_TexCoord;                           " \
      "                                                     " \
      "uniform mat4 u_Projection;                           " \
      "uniform mat4 u_Model;                                " \
      "                                                     " \
      "varying vec2 v_TexCoord;                             " \
      "                                                     " \
      "void main()                                          " \
      "{                                                    " \
      "  gl_Position = u_Projection * u_Model * vec4(a_Position, 0, 1); " \
      "  v_TexCoord = a_TexCoord;                           " \
      "}                                                    " \
      "\n#endif\n                                           " \
      "\n#ifdef FRAGMENT\n                                  " \
      "varying vec2 v_TexCoord;                             " \
      "                                                     " \
      "uniform sampler2D u_Texture;                         " \
      "                                                     " \
      "void main()                                          " \
      "{                                                    " \
      "  vec4 tex = texture2D(u_Texture, v_TexCoord);       " \
      "  gl_FragColor = tex;                                " \
      "}                                                    " \
      "\n#endif\n                                           ";

    m_blend = true;
    m_depthtest = false;
    m_backfacecull = true;
  }
  else if(_type == NDC_SHADER)
  {
    m_source =
      "\n#ifdef VERTEX\n                                    " \
      "attribute vec4 a_Position;                           " \
      "                                                     " \
      "varying vec2 v_TexCoord;                             " \
      "                                                     " \
      "void main()                                          " \
      "{                                                    " \
      "  vec4 p = a_Position;                               " \
      "  vec2 c = vec2(0, 0);                               " \
      "                                                     " \
      "  if(p.x < 0.0) { p.x = -1.0; c.x = 0.0; }           " \
      "  else if(p.x >= 0.0) { p.x = 1.0; c.x = 1.0; }      " \
      "  if(p.y < 0.0) { p.y = -1.0; c.y = 0.0; }           " \
      "  else if(p.y >= 0.0) { p.y = 1.0; c.y = 1.0; }      " \
      "                                                     " \
      "  p.z = 0.0;                                         " \
      "  p.w = 1.0;                                         " \
      "                                                     " \
      "  gl_Position = p;                                   " \
      "  v_TexCoord = c;                                    " \
      "}                                                    " \
      "\n#endif\n                                           " \
      "\n#ifdef FRAGMENT\n                                  " \
      "varying vec2 v_TexCoord;                             " \
      "                                                     " \
      "uniform sampler2D u_Texture;                         " \
      "                                                     " \
      "void main()                                          " \
      "{                                                    " \
      "  vec4 tex = texture2D(u_Texture, v_TexCoord);       " \
      "  gl_FragColor = tex;                                " \
      "}                                                    " \
      "\n#endif\n                                           ";

/*
    m_source =
      "\n#ifdef VERTEX\n                                    " \
      "varying vec2 v_TexCoord;                             " \
      "                                                     " \
      "void main()                                          " \
      "{                                                    " \
      "  vec4 p = vec4(0, 0, 0, 1);                         " \
      "  vec2 c = vec2(0, 0);                               " \
      "                                                     " \
      "  if(gl_VertexID == 0) p = vec4(-1, 1, 0, 1);        " \
      "  else if(gl_VertexID == 1) p = vec4(-1, -1, 0, 1);  " \
      "  else if(gl_VertexID == 2) p = vec4(1, -1, 0, 1);   " \
      "  else if(gl_VertexID == 3) p = vec4(1, -1, 0, 1);   " \
      "  else if(gl_VertexID == 4) p = vec4(1, 1, 0, 1);    " \
      "  else if(gl_VertexID == 5) p = vec4(-1, 1, 0, 1);   " \
      "                                                     " \
      "  if(p.x < 0.0) c.x = 0.0;                           " \
      "  else if(p.x >= 0.0) c.x = 1.0;                     " \
      "  if(p.y < 0.0) c.y = 0.0;                           " \
      "  else if(p.y >= 0.0) c.y = 1.0;                     " \
      "                                                     " \
      "  gl_Position = p;                                   " \
      "  v_TexCoord = c;                                    " \
      "}                                                    " \
      "\n#endif\n                                           " \
      "\n#ifdef FRAGMENT\n                                  " \
      "varying vec2 v_TexCoord;                             " \
      "                                                     " \
      "uniform sampler2D u_Texture;                         " \
      "                                                     " \
      "void main()                                          " \
      "{                                                    " \
      "  vec4 tex = texture2D(u_Texture, v_TexCoord);       " \
      "  gl_FragColor = tex;                                " \
      "}                                                    " \
      "\n#endif\n                                           ";
*/
  }
  else
  {
    throw Exception("Invalid shader type specified");
  }

  m_dirty = true;
}

void Shader::depth_test(bool _depthtest)
{
  m_depthtest = _depthtest;
}

void Shader::backface_cull(bool _backfacecull)
{
  m_backfacecull = _backfacecull;
}

void Shader::blend(bool _blend)
{
  m_blend = _blend;
}

void Shader::render(RenderTexture& _rendertexture)
{
  GLint viewport[4] = {0};
  glGetIntegerv(GL_VIEWPORT, viewport);

  glBindFramebuffer(GL_FRAMEBUFFER, _rendertexture.id());
  glViewport(0, 0, _rendertexture.width(), _rendertexture.height());

  render();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}

void Shader::render()
{
  id();
  int count = -1;

  for(sys::linked_list<Attribute>::iterator it = m_attributes.begin();
    it != m_attributes.end(); ++it)
  {
    if(count == -1)
    {
      count = it->buffer->size();
    }
    else
    {
      if(count != it->buffer->size())
      {
        throw Exception("Jagged buffers not supported");
      }
    }

    if(it->location == -2)
    {
      Debug::out("glGetAttribLocation");
      it->location = glGetAttribLocation(id(), it->name.unsafe_raw());
    }

    if(it->location < 0) continue;

    glBindBuffer(GL_ARRAY_BUFFER, it->buffer->id());

    glVertexAttribPointer(it->location, it->buffer->components(), GL_FLOAT,
      GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(it->location);
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glUseProgram(id());

  int texcount = -1;

  for(sys::linked_list<Uniform>::iterator it = m_uniforms.begin();
    it != m_uniforms.end(); ++it)
  {
    if(it->location == -2)
    {
      Debug::out("glGetUniformLocation");
      it->location = glGetUniformLocation(id(), it->name.unsafe_raw());
    }

    if(it->location < 0) continue;

    if(it->type == E_MAT4)
    {
      glUniformMatrix4fv(it->location, 1, GL_FALSE, rend::value_ptr(it->mat4_value));
    }
    else if(it->type == E_VEC4)
    {
      glUniform4f(it->location, it->vec4_value.x, it->vec4_value.y, it->vec4_value.z, it->vec4_value.w);
    }
    else if(it->type == E_VEC3)
    {
      glUniform3f(it->location, it->vec3_value.x, it->vec3_value.y, it->vec3_value.z);
    }
    else if(it->type == E_VEC2)
    {
      glUniform2f(it->location, it->vec2_value.x, it->vec2_value.y);
    }
    else if(it->type == E_INT)
    {
      glUniform1i(it->location, it->int_value);
    }
    else if(it->type == E_TEXTURE)
    {
      ++texcount;

      glActiveTexture(GL_TEXTURE0 + texcount);

      if(it->texture_value)
      {
        glBindTexture(GL_TEXTURE_2D, it->texture_value->id());
      }
      else if(it->rendertexture_value)
      {
        glBindTexture(GL_TEXTURE_2D, it->rendertexture_value->texture_id());
      }
      else
      {
        throw Exception("Invalid texture uniform");
      }

      glUniform1i(it->location, texcount);
    }
    else
    {
      throw Exception("Invalid uniform type");
    }
  }

  if(m_blend)
  {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }

  if(m_depthtest)
  {
    glEnable(GL_DEPTH_TEST);
  }

  if(m_backfacecull)
  {
    glEnable(GL_CULL_FACE);
  }

  glDrawArrays(GL_TRIANGLES, 0, count);

  for(int i = texcount; i >= 0; --i)
  {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  for(sys::linked_list<Attribute>::iterator it = m_attributes.begin();
    it != m_attributes.end(); ++it)
  {
    if(it->location < 0) continue;
    glDisableVertexAttribArray(it->location);
  }

  if(m_depthtest)
  {
    glDisable(GL_DEPTH_TEST);
  }

  if(m_blend)
  {
    glDisable(GL_BLEND);
  }

  if(m_backfacecull)
  {
    glDisable(GL_CULL_FACE);
  }

  glUseProgram(0);
}

void Shader::uniform(const sys::string& _name, Texture& _value)
{
  sys::ptr<Uniform> u = find_uniform(_name, E_TEXTURE);
  u->texture_value = &_value;
  u->rendertexture_value.reset();
}

void Shader::uniform(const sys::string& _name, RenderTexture& _value)
{
  sys::ptr<Uniform> u = find_uniform(_name, E_TEXTURE);
  u->rendertexture_value = &_value;
  u->texture_value.reset();
}

void Shader::uniform(const sys::string& _name, const mat4& _value)
{
  sys::ptr<Uniform> u = find_uniform(_name, E_MAT4);
  u->mat4_value = _value;
}

void Shader::uniform(const sys::string& _name, vec4 _value)
{
  sys::ptr<Uniform> u = find_uniform(_name, E_VEC4);
  u->vec4_value = _value;
}

void Shader::uniform(const sys::string& _name, vec3 _value)
{
  sys::ptr<Uniform> u = find_uniform(_name, E_VEC3);
  u->vec3_value = _value;
}

void Shader::uniform(const sys::string& _name, vec2 _value)
{
  sys::ptr<Uniform> u = find_uniform(_name, E_VEC2);
  u->vec2_value = _value;
}

void Shader::uniform(const sys::string& _name, int _value)
{
  sys::ptr<Uniform> u = find_uniform(_name, E_INT);
  u->int_value = _value;
}

sys::ptr<Shader::Uniform> Shader::find_uniform(const sys::string& _name, int _type)
{
  for(sys::linked_list<Uniform>::iterator it = m_uniforms.begin();
    it != m_uniforms.end(); ++it)
  {
    if(it->name == _name)
    {
      if(it->type != _type)
      {
        throw Exception("Invalid uniform type");
      }

      return &(*it);
    }
  }

  Uniform u;
  u.name = _name;
  u.type = _type;
  u.location = -2;

  return m_uniforms.push_back(u);
}

void Shader::attribute(const sys::string& _name, Buffer& _buffer)
{
  for(sys::linked_list<Attribute>::iterator it = m_attributes.begin();
    it != m_attributes.end(); ++it)
  {
    if(it->name == _name)
    {
      it->buffer = &_buffer;

      return;
    }
  }

  Attribute a;
  a.name = _name;
  a.location = -2;
  a.buffer = &_buffer;
  m_attributes.push_back(a);
}

GLuint Shader::id()
{
  if(!m_id)
  {
    Debug::out("glCreateProgram");
    m_id = glCreateProgram();

    if(!m_id)
    {
      throw Exception("Failed to create shader program");
    }
  }

  if(m_dirty)
  {
    for(sys::linked_list<Attribute>::iterator it = m_attributes.begin();
      it != m_attributes.end(); ++it)
    {
      it->location = -2;
    }

    for(sys::linked_list<Uniform>::iterator it = m_uniforms.begin();
      it != m_uniforms.end(); ++it)
    {
      it->location = -2;
    }

    sys::string source;
    //source += "\n#version 150\n";
    source += "\n#define VERTEX\n";
    source += m_source;

    Debug::out("glCreateShader");
    GLuint vsId = glCreateShader(GL_VERTEX_SHADER);

    const GLchar *src = &source[0];
    Debug::out("glShaderSource");
    glShaderSource(vsId, 1, &src, NULL);

    Debug::out("glCompileShader");
    glCompileShader(vsId);

    GLint success = 0;
    glGetShaderiv(vsId, GL_COMPILE_STATUS, &success);

    if(!success)
    {
      GLint len = 0;
      glGetShaderiv(vsId, GL_INFO_LOG_LENGTH, &len);
      sys::vector<GLchar> log(len);
      glGetShaderInfoLog(vsId, log.size(), NULL, &log[0]);
      glDeleteShader(vsId);

      throw Exception("Failed to compile vertex shader ["
        + sys::string(&log[0]) + "]");
    }

    source = "";
    //source += "\n#version 150\n";
    source += "\n#define FRAGMENT\n";
    source += m_source;

    Debug::out("glCreateShader");
    GLuint fsId = glCreateShader(GL_FRAGMENT_SHADER);

    src = &source[0];
    Debug::out("glShaderSource");
    glShaderSource(fsId, 1, &src, NULL);

    Debug::out("glCompileShader");
    glCompileShader(fsId);

    success = 0;
    glGetShaderiv(fsId, GL_COMPILE_STATUS, &success);

    if(!success)
    {
      GLint len = 0;
      glGetShaderiv(fsId, GL_INFO_LOG_LENGTH, &len);
      sys::vector<GLchar> log(len);
      glGetShaderInfoLog(fsId, log.size(), NULL, &log[0]);
      glDeleteShader(vsId);
      glDeleteShader(fsId);

      throw Exception("Failed to compile fragment shader ["
        + sys::string(&log[0]) + "]");
    }

    glAttachShader(m_id, vsId);
    glAttachShader(m_id, fsId);

    Debug::out("glLinkProgram");
    glLinkProgram(m_id);
    glDetachShader(m_id, vsId);

    Debug::out("glDeleteShader");
    glDeleteShader(vsId);
    glDetachShader(m_id, fsId);

    Debug::out("glDeleteShader");
    glDeleteShader(fsId);

    success = 0;
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);

    if(!success)
    {
      GLint len = 0;
      glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &len);
      sys::vector<GLchar> log(len);
      glGetProgramInfoLog(m_id, log.size(), NULL, &log[0]);
      glDeleteShader(vsId);
      glDeleteShader(fsId);

      throw Exception("Failed to link shader program ["
        + sys::string(&log[0]) + "]");
    }

    m_dirty = false;
  }

  return m_id;
}

}

