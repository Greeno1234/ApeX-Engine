#include "ModelShader.h"
#include "RenderTexture.h"
#include "Model.h"
#include "Mesh.h"
#include "Animation.h"

namespace rend
{

ModelShader::ModelShader()
  : m_color(1, 1, 1, 1)
  , m_projection(1.0f)
  , m_view(1.0f)
  , m_model(1.0f)
  , m_textures(true)
  , m_lighting(true)
{
  sys::string vertsrc =
    "attribute vec4 a_Position;                                    " \
    "attribute vec2 a_TexCoord;                                    " \
    "attribute vec3 a_Normal;                                      " \
    "                                                              " \
    "uniform mat4 u_Projection;                                    " \
    "uniform mat4 u_View;                                          " \
    "uniform mat4 u_Model;                                         " \
    "                                                              " \
    "varying vec2 v_TexCoord;                                      " \
    "varying vec3 v_Normal;                                        " \
    "varying vec3 v_FragPos;                                       " \
    "                                                              " \
    "void main()                                                   " \
    "{                                                             " \
    "  gl_Position = u_Projection * u_View * u_Model * a_Position; " \
    "  v_TexCoord = a_TexCoord;                                    " \
    "  v_Normal = vec3(u_Model * vec4(a_Normal, 0));               " \
    "  v_FragPos = vec3(u_Model * a_Position);                     " \
    "}                                                             ";

  sys::string fragsrc =
    "varying vec2 v_TexCoord;                             " \
    "varying vec3 v_Normal;                               " \
    "varying vec3 v_FragPos;                              " \
    "                                                     " \
    "uniform int u_EnableTexture;                         " \
    "uniform int u_EnableLighting;                        " \
    "uniform sampler2D u_Texture;                         " \
    "uniform vec4 u_Diffuse;                              " \
    "uniform vec4 u_Color;                                " \
    "uniform vec3 u_ViewPos;                              " \
    "                                                     " \
    "void main()                                          " \
    "{                                                    " \
    "  vec4 tex = vec4(1, 1, 1, 1);                       " \
    "  if(u_EnableTexture == 1)                           " \
    "  {                                                  " \
    "    tex *= texture2D(u_Texture, v_TexCoord);         " \
    "  }                                                  " \
    "  tex *= u_Diffuse;                                  " \
    "  tex *= u_Color;                                    " \
    "  if(u_EnableLighting == 1)                          " \
    "  {                                                  " \
    "    vec3 lighting = vec3(0, 0, 0);                   " \
    "    vec3 lightpos = vec3(100, 100, 100);             " \
    "    vec3 lightdir = normalize(lightpos - v_FragPos); " \
    "    vec3 normal = normalize(v_Normal);               " \
    "    float diff = max(dot(normal, lightdir), 0.0);    " \
    "    lighting += vec3(diff);                          " \
    "    vec3 viewdir = normalize(u_ViewPos - v_FragPos); " \
    "    vec3 reflectdir = reflect(-lightdir, normal);    " \
    "    float spec = pow(max(dot(viewdir, reflectdir),   " \
    "      0.0), 32.0);                                   " \
    "    lighting += vec3(spec);                          " \
    "    lighting = min(lighting, vec3(1, 1, 1));         " \
    "    lighting = max(lighting, vec3(0.2, 0.2, 0.2));   " \
    "    tex *= vec4(lighting, 1);                        " \
    "  }                                                  " \
    "  gl_FragColor = tex;                                " \
    "}                                                    ";

  m_shader.load(vertsrc, fragsrc);

  m_shader.depth_test(true);
  m_shader.backface_cull(true);

  m_shader.uniform("u_Projection", m_projection);
  m_shader.uniform("u_View", m_view);
  m_shader.uniform("u_Model", m_model);
}

void ModelShader::projection(const mat4& _projection)
{
  m_projection = _projection;
  m_shader.uniform("u_Projection", _projection);
}

mat4 ModelShader::projection() const
{
  return m_projection;
}

void ModelShader::view(const mat4& _view)
{
  m_view = _view;
  m_shader.uniform("u_View", _view);
}

mat4 ModelShader::view() const
{
  return m_view;
}

void ModelShader::model(const mat4& _model)
{
  m_model = _model;
  m_shader.uniform("u_Model", _model);
}

mat4 ModelShader::model() const
{
  return m_model;
}

void ModelShader::model(Model& _model)
{
  m_modeldat = &_model;
}

void ModelShader::animation(Animation& _animation)
{
  m_animation = &_animation;
}

void ModelShader::render()
{
  m_shader.uniform("u_EnableLighting", m_lighting ? 1 : 0);

  vec3 viewpos(inverse(m_view) * vec4(0.0f, 0.0f, 0.0f, 1.0f));
  m_shader.uniform("u_ViewPos", viewpos);

  for(sys::linked_list<Part>::iterator
    pit = m_modeldat->parts_begin(); pit != m_modeldat->parts_end(); ++pit)
  {
    if(m_animation)
    {
      Transform t;
      t.part = pit->name();
      m_animation->apply(t, m_frame);
      m_shader.uniform("u_Model", m_model * t.model(*pit));
    }

    m_shader.uniform("u_Color", m_color * pit->color());

    for(sys::linked_list<MaterialGroup>::iterator
      mgit = pit->mgs_begin(); mgit != pit->mgs_end(); ++mgit)
    {
      m_shader.attribute("a_Position", *mgit->mesh.positions());
      m_shader.attribute("a_TexCoord", *mgit->mesh.texcoords());
      m_shader.attribute("a_Normal", *mgit->mesh.normals());

      m_shader.uniform("u_Diffuse", mgit->material->diffuse);

      if(mgit->material->texture && m_textures)
      {
        m_shader.uniform("u_EnableTexture", 1);
        m_shader.uniform("u_Texture", *mgit->material->texture);
      }
      else
      {
        m_shader.uniform("u_EnableTexture", 0);
      }

      m_shader.render();
    }
  }
}

void ModelShader::render(RenderTexture& _rendertexture)
{
  GLint viewport[4] = {0};
  glGetIntegerv(GL_VIEWPORT, viewport);

  glBindFramebuffer(GL_FRAMEBUFFER, _rendertexture.id());
  glViewport(0, 0, _rendertexture.width(), _rendertexture.height());

  render();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}

void ModelShader::frame(float _frame)
{
  if(!m_animation)
  {
    m_frame = 0;

    return;
  }

  m_frame = _frame;

  if(m_frame >= m_animation->frames())
  {
    m_frame = m_animation->frames() - 1;
  }
}

sys::ptr<Part> ModelShader::intersect(const Ray& _ray, const mat4& _model,
  float& _distance) const
{
  sys::ptr<Part> rtn;

  _distance = FLT_MAX;
  if(!m_modeldat) return NULL;

  for(sys::linked_list<Part>::iterator pit = m_modeldat->parts_begin();
    pit != m_modeldat->parts_end(); ++pit)
  {
    mat4 model(_model);

    if(m_animation)
    {
      Transform t;
      t.part = pit->name();
      m_animation->apply(t, m_frame);
      //invModel = invModel * m_animation->model(*pit, m_frame);
      model = model * t.model(*pit);
    }

/*
    Ray r(_ray);

    invModel = inverse(invModel);

    r.origin = invModel *
      rend::vec4(r.origin, 1);

    r.direction = invModel *
      rend::vec4(r.direction, 0);
*/

    for(sys::linked_list<MaterialGroup>::iterator mit = pit->mgs_begin();
      mit != pit->mgs_end(); ++mit)
    {
      float nd = FLT_MAX;

      if(mit->mesh.intersect(_ray, model, nd))
      {
        if(nd < _distance)
        {
          rtn = &*pit;
          _distance = nd;
        }
      }
    }
  }

/*
  if(rtn)
  {
    sys::out("Intersect: " + rtn->name() + "\n");
  }
*/

  return rtn;
}

void ModelShader::color(const vec4& _color)
{
  m_color = _color;
}

void ModelShader::depth_test(bool _depthtest)
{
  m_shader.depth_test(_depthtest);
}

void ModelShader::lighting(bool _lighting)
{
  m_lighting = _lighting;
}

bool ModelShader::lighting() const
{
  return m_lighting;
}

void ModelShader::textures(bool _textures)
{
  m_textures = _textures;
}

bool ModelShader::textures() const
{
  return m_textures;
}

}

