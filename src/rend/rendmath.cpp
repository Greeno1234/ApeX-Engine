#include "rendmath.h"

#include <sys/io.h>
#include <sys/string.h>

namespace rend
{

mat4 rotate_xyz(const mat4& _m, const vec3& _angle)
{
  mat4 rtn(_m);

  rtn = rend::rotate(rtn, rend::radians(_angle.y), vec3(0, 1, 0));
  rtn = rend::rotate(rtn, rend::radians(_angle.x), vec3(1, 0, 0));
  rtn = rend::rotate(rtn, rend::radians(_angle.z), vec3(0, 0, 1));

  return rtn;
}

void Ray::debug() const
{
  sys::out("Ray: origin ["
    + sys::string(origin.x) + ", "
    + sys::string(origin.y) + ", "
    + sys::string(origin.z) + "] direction ["
    + sys::string(direction.x) + ", "
    + sys::string(direction.y) + ", "
    + sys::string(direction.z) + "]"
    + "\n");
}

}

