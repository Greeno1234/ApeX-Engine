#include "TextShader.h"
#include "RenderTexture.h"
#include "Font.h"

namespace rend
{

TextShader::TextShader()
  : m_projection(1.0f)
  , m_view(1.0f)
  , m_model(1.0f)
  , m_mesh(GUI_QUAD_MESH)
{
  sys::string vertsrc =
    "attribute vec4 a_Position;                                    " \
    "attribute vec2 a_TexCoord;                                    " \
    "                                                              " \
    "uniform mat4 u_Projection;                                    " \
    "uniform mat4 u_View;                                          " \
    "uniform mat4 u_Model;                                         " \
    "uniform int u_Flip;                                           " \
    "                                                              " \
    "varying vec2 v_TexCoord;                                      " \
    "                                                              " \
    "void main()                                                   " \
    "{                                                             " \
    "  vec4 position = a_Position;                                 " \
    "  if(u_Flip == 1)                                             " \
    "  {                                                           " \
    "    position.y = -position.y;                                 " \
    "  }                                                           " \
    "  gl_Position = u_Projection * u_View * u_Model * position;   " \
    "  v_TexCoord = a_TexCoord;                                    " \
    "}                                                             ";

  sys::string fragsrc =
    "varying vec2 v_TexCoord;                             " \
    "                                                     " \
    "uniform sampler2D u_Texture;                         " \
    "                                                     " \
    "void main()                                          " \
    "{                                                    " \
    "  vec4 tex = texture2D(u_Texture, v_TexCoord);       " \
    "  gl_FragColor = tex;                                " \
    "}                                                    ";

  m_shader.load(vertsrc, fragsrc);

  m_shader.depth_test(true);
  m_shader.backface_cull(true);

  m_shader.uniform("u_Projection", m_projection);
  m_shader.uniform("u_View", m_view);
  m_shader.uniform("u_Model", m_model);
}

void TextShader::projection(const mat4& _projection)
{
  m_projection = _projection;
  m_shader.uniform("u_Projection", _projection);
}

mat4 TextShader::projection() const
{
  return m_projection;
}

void TextShader::view(const mat4& _view)
{
  m_view = _view;
  m_shader.uniform("u_View", _view);
}

mat4 TextShader::view() const
{
  return m_view;
}

void TextShader::model(const mat4& _model)
{
  m_model = _model;
}

mat4 TextShader::model() const
{
  return m_model;
}

void TextShader::font(Font& _font)
{
  m_font = &_font;
}

void TextShader::text(const sys::string& _text)
{
  m_text = _text;
}

void TextShader::render()
{
  m_shader.attribute("a_Position", *m_mesh.positions());
  m_shader.uniform("u_Texture", *m_font->texture());

  mat4 offset(m_model);

  // Center text in a 3D coordinate system.
  // Y axis of mesh also assumes top == 0, so flip that.
  if(is_perspective(m_projection))
  {
    vec2 fontsize(m_font->size(m_text));
    offset = translate(offset, vec3(fontsize.x * -0.5f, fontsize.y * 0.5f, 0.0f));
    m_shader.uniform("u_Flip", 1);
  }
  else
  {
    m_shader.uniform("u_Flip", 0);
  }

  float x = 0;

  for(size_t ci = 0; ci < m_text.length(); ++ci)
  {
    sys::ptr<Glyph> glyph = m_font->glyph(m_text[ci]);

    m_shader.attribute("a_TexCoord", glyph->buffer);

    mat4 model(offset);
    model = translate(model, vec3(x, 0, 0.0f));
    model = scale(model, vec3(glyph->rect.z, glyph->rect.w, 1.0f));
    m_shader.uniform("u_Model", model);
    m_shader.render();

    x += glyph->rect.z + 1.0f;
  }
}

void TextShader::render(RenderTexture& _rendertexture)
{
  GLint viewport[4] = {0};
  glGetIntegerv(GL_VIEWPORT, viewport);

  glBindFramebuffer(GL_FRAMEBUFFER, _rendertexture.id());
  glViewport(0, 0, _rendertexture.width(), _rendertexture.height());

  render();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}

bool TextShader::is_perspective(const mat4& _projection)
{
  vec4 a = _projection * vec4(0, 0, 0, 1);
  vec4 b = _projection * vec4(0, 1, 0, 1);

  if(b.y > a.y) return true;

  return false;
}

sys::ptr<Shader> TextShader::shader()
{
  return &m_shader;
}

}

