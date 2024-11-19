#include "Mesh.h"
#include "rendmath.h"
#include "Exception.h"
#include "utils.h"

#include "glm/gtx/intersect.hpp"

namespace rend
{

Mesh::Mesh()
{ }

Mesh::Mesh(int _type)
{
  load(_type);
}

void Mesh::load(int _type)
{
  if(_type == TRIANGLE_MESH)
  {
    m_positions.add(rend::vec3(0.0f, 0.5f, 0));
    m_positions.add(rend::vec3(-0.5f, -0.5f, 0));
    m_positions.add(rend::vec3(0.5f, -0.5f, 0));

    m_texcoords.add(rend::vec2(0.5f, 1));
    m_texcoords.add(rend::vec2(0, 0));
    m_texcoords.add(rend::vec2(1, 0));

    m_normals.add(rend::vec3(0, 0, 1));
    m_normals.add(rend::vec3(0, 0, 1));
    m_normals.add(rend::vec3(0, 0, 1));
  }
  else if(_type == QUAD_MESH)
  {
    m_positions.add(rend::vec3(-0.5f, 0.5f, 0));
    m_positions.add(rend::vec3(-0.5f, -0.5f, 0));
    m_positions.add(rend::vec3(0.5f, -0.5f, 0));
    m_positions.add(rend::vec3(0.5f, -0.5f, 0));
    m_positions.add(rend::vec3(0.5f, 0.5f, 0));
    m_positions.add(rend::vec3(-0.5, 0.5f, 0));

    m_texcoords.add(rend::vec2(0, 1));
    m_texcoords.add(rend::vec2(0, 0));
    m_texcoords.add(rend::vec2(1, 0));
    m_texcoords.add(rend::vec2(1, 0));
    m_texcoords.add(rend::vec2(1, 1));
    m_texcoords.add(rend::vec2(0, 1));

    m_normals.add(rend::vec3(0, 0, 1));
    m_normals.add(rend::vec3(0, 0, 1));
    m_normals.add(rend::vec3(0, 0, 1));
    m_normals.add(rend::vec3(0, 0, 1));
    m_normals.add(rend::vec3(0, 0, 1));
    m_normals.add(rend::vec3(0, 0, 1));
  }
  else if(_type == GUI_QUAD_MESH)
  {
    m_positions.add(rend::vec3(0, 0, 0));
    m_positions.add(rend::vec3(0, 1, 0));
    m_positions.add(rend::vec3(1, 1, 0));
    m_positions.add(rend::vec3(1, 1, 0));
    m_positions.add(rend::vec3(1, 0, 0));
    m_positions.add(rend::vec3(0, 0, 0));

    m_texcoords.add(rend::vec2(0, 1));
    m_texcoords.add(rend::vec2(0, 0));
    m_texcoords.add(rend::vec2(1, 0));
    m_texcoords.add(rend::vec2(1, 0));
    m_texcoords.add(rend::vec2(1, 1));
    m_texcoords.add(rend::vec2(0, 1));

    m_normals.add(rend::vec3(0, 0, 1));
    m_normals.add(rend::vec3(0, 0, 1));
    m_normals.add(rend::vec3(0, 0, 1));
    m_normals.add(rend::vec3(0, 0, 1));
    m_normals.add(rend::vec3(0, 0, 1));
    m_normals.add(rend::vec3(0, 0, 1));
  }
  else if(_type == NDC_QUAD_MESH)
  {
    m_positions.add(rend::vec3(-1, 1, 0));
    m_positions.add(rend::vec3(-1, -1, 0));
    m_positions.add(rend::vec3(1, -1, 0));
    m_positions.add(rend::vec3(1, -1, 0));
    m_positions.add(rend::vec3(1, 1, 0));
    m_positions.add(rend::vec3(-1, 1, 0));

    m_texcoords.add(rend::vec2(0, 1));
    m_texcoords.add(rend::vec2(0, 0));
    m_texcoords.add(rend::vec2(1, 0));
    m_texcoords.add(rend::vec2(1, 0));
    m_texcoords.add(rend::vec2(1, 1));
    m_texcoords.add(rend::vec2(0, 1));

    m_normals.add(rend::vec3(0, 0, 1));
    m_normals.add(rend::vec3(0, 0, 1));
    m_normals.add(rend::vec3(0, 0, 1));
    m_normals.add(rend::vec3(0, 0, 1));
    m_normals.add(rend::vec3(0, 0, 1));
    m_normals.add(rend::vec3(0, 0, 1));
  }
  else
  {
    throw Exception("Invalid mesh type specified");
  }
}

sys::ptr<Buffer> Mesh::positions()
{
  return &m_positions;
}

sys::ptr<Buffer> Mesh::texcoords()
{
  return &m_texcoords;
}

sys::ptr<Buffer> Mesh::normals()
{
  return &m_normals;
}

bool Mesh::intersect(const Ray& _ray, const mat4& _model, float& _distance) const
{
  bool rtn = false;
  _distance = FLT_MAX;

  Ray ray(_ray);

  mat4 invmodel = inverse(_model);

  ray.origin = invmodel *
    rend::vec4(ray.origin, 1);

  ray.direction = invmodel *
    rend::vec4(ray.direction, 0);

  for(size_t fi = 0; fi < m_faces.size(); ++fi)
  {
    Face f = m_faces[fi];
    vec2 bary;
    float nd = 0;

    if(intersectRayTriangle(ray.origin, ray.direction,
      f.a.position, f.b.position, f.c.position, bary, nd) == true)
    {
      if(nd < 0) continue;

      rtn = true;

      if(nd < _distance)
      {
        _distance = nd;
      }
    }
  }

  return rtn;
}

void Mesh::add(const Face& _face)
{
  m_faces.push_back(_face);
}

sys::vector<Face>::iterator Mesh::faces_begin()
{
  return m_faces.begin();
}

sys::vector<Face>::iterator Mesh::faces_end()
{
  return m_faces.end();
}

}

