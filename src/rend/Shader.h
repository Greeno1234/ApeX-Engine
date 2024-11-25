#ifndef REND_SHADER_H
#define REND_SHADER_H

#include "rendmath.h"
#include "rendgl.h"

#include <sys/string.h>
#include <sys/memory.h>
#include <sys/linkedlist.h>

namespace rend
{

struct Texture;
struct Buffer;
struct RenderTexture;

enum ShaderType
{
  TEXTURE_SHADER = 0,
  TEXTURE_LIGHT_SHADER,
  COLOR_SHADER,
  COLOR_LIGHT_SHADER,
  GUI_SHADER,
  NDC_SHADER
};

struct Shader : sys::base
{
  Shader();
  Shader(const Shader& _copy);
  Shader& operator=(const Shader& _assign);
  ~Shader();

  Shader(int _type);
  //Shader(const sys::string& _path);
  //Shader(const sys::string& _vertsrc, const sys::string& _fragsrc);

  void load(int _type);
  //void load(const sys::string& _path);
  void load(const sys::string& _vertsrc, const sys::string& _fragsrc);

  void attribute(const sys::string& _name, Buffer& _buffer);

  void uniform(const sys::string& _name, int _value);
  void uniform(const sys::string& _name, float _value);
  void uniform(const sys::string& _name, vec2 _value);
  void uniform(const sys::string& _name, vec3 _value);
  void uniform(const sys::string& _name, vec4 _value);
  void uniform(const sys::string& _name, const mat4& _value);
  void uniform(const sys::string& _name, Texture& _value);
  void uniform(const sys::string& _name, RenderTexture& _value);

  void blend(bool _blend);
  void depth_test(bool _depthtest);
  void backface_cull(bool _backfacecull);

  void render();
  void render(RenderTexture& _rendertexture);
  //void render(Mesh& _mesh);
  //void render(Model& _model);
  //void render(AnimatedModel& _model, float _frame);

  GLuint id();

private:
  struct Uniform;
  struct Attribute;

  sys::string m_source;
  sys::linked_list<Attribute> m_attributes;
  sys::linked_list<Uniform> m_uniforms;

  sys::zero<bool> m_dirty;
  sys::zero<GLuint> m_id;
  sys::zero<GLuint> m_vaoid;
  sys::zero<bool> m_blend;
  sys::zero<bool> m_depthtest;
  sys::zero<bool> m_backfacecull;

  sys::ptr<Uniform> find_uniform(const sys::string& _name, int _type);
};

}

#endif

