#include "rendmath.h"

#include <sys/memory.h>

namespace rend
{

struct Shader;
struct Mesh
struct RenderTarget;
struct Texture;

struct Renderer
{
  sys::ptr<Shader> shader;
  sys::ptr<Mesh> mesh;
  sys::ptr<RenderTarget> render_target;

  sys::ptr<Texture> texture0;
  sys::ptr<Texture> texture1;
  sys::ptr<Texture> texture2;
  sys::ptr<Texture> texture3;

  mat4 projection_matrix;
  mat4 view_matrix;
  mat4 model_matrix;

  sys::zero<bool> cull_backface;
  sys::zero<bool> depth_test;

  void render();
};

}

