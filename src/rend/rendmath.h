#ifndef REND_RENDMATH_H
#define REND_RENDMATH_H

#define GLM_FORCE_CTOR_INIT
#include "glm/glm.hpp"
#include "glm/ext.hpp"

namespace rend
{

using namespace glm;

struct Ray
{
  vec3 origin;
  vec3 direction;

  void debug() const;
};

struct Triangle
{
  vec3 a;
  vec3 b;
  vec3 c;
};

mat4 rotate_xyz(const mat4& _m, const vec3& _angle);

}

#endif

