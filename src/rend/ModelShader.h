#include "Shader.h"
#include "rendmath.h"

#include <sys/memory.h>

namespace rend
{

struct RenderTexture;
struct Model;
struct Animation;
struct Part;

struct ModelShader : sys::base
{
  ModelShader();

  void projection(const mat4& _projection);
  mat4 projection() const;

  void view(const mat4& _view);
  mat4 view() const;

  void model(const mat4& _model);
  mat4 model() const;

  void model(Model& _model);

  void animation(Animation& _animation);
  void frame(float _frame);

  void render();
  void render(RenderTexture& _rendertexture);

  sys::ptr<Part> intersect(const Ray& _ray, const mat4& _model,
    float& _distance) const;

  void color(const vec4& _color);
  void depth_test(bool _depthtest);

  void lighting(bool _lighting);
  bool lighting() const;

  void textures(bool _textures);
  bool textures() const;

private:
  Shader m_shader;
  sys::ptr<Model> m_modeldat;
  sys::ptr<Animation> m_animation;
  sys::zero<float> m_frame;
  vec4 m_color;

  mat4 m_projection;
  mat4 m_view;
  mat4 m_model;

  sys::zero<bool> m_lighting;
  sys::zero<bool> m_textures;

};

}

