#include "rendmath.h"

namespace rend
{

struct Face
{
  vec3 pa;
  vec3 pb;
  vec3 pc;

  vec2 tca;
  vec2 tcb;
  vec2 tcc;

  vec3 na;
  vec3 nb;
  vec3 nc;

  vec3 normal();

private:
  vec3 m_normal;
};

}

