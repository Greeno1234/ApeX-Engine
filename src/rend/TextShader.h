#include "Shader.h"
#include "rendmath.h"
#include "Mesh.h"

#include <sys/memory.h>
#include <sys/string.h>

namespace rend
{

struct RenderTexture;
struct Font;

struct TextShader : sys::base
{
  TextShader();

  void projection(const mat4& _projection);
  mat4 projection() const;

  void view(const mat4& _view);
  mat4 view() const;

  void model(const mat4& _model);
  mat4 model() const;

  void font(Font& _font);
  void text(const sys::string& _text);

  void render();
  void render(RenderTexture& _rendertexture);

  sys::ptr<Shader> shader();

private:
  rend::Mesh m_mesh;
  Shader m_shader;
  sys::ptr<Font> m_font;
  sys::string m_text;

  mat4 m_projection;
  mat4 m_view;
  mat4 m_model;

  bool is_perspective(const mat4& _projection);

};

}
