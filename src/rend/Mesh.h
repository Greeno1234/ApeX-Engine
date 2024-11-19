#ifndef REND_MESH_H
#define REND_MESH_H

#include "rendmath.h"
#include "Buffer.h"

#include <sys/memory.h>
#include <sys/vector.h>

namespace rend
{

enum MeshType
{
  TRIANGLE_MESH = 0,
  QUAD_MESH,
  GUI_QUAD_MESH,
  NDC_QUAD_MESH
};

struct Vertex
{
  vec3 position;
  vec2 texcoord;
  vec3 normal;
};

struct Face
{
  Vertex a;
  Vertex b;
  Vertex c;
};

struct Mesh
{
  Mesh();

  Mesh(int _type);
  void load(int _type);

/*
  Mesh(const sys::vector<unsigned char>& _data);
  void load(const sys::vector<unsigned char>& _data);
*/

  void add(const Face& _face);

  sys::ptr<Buffer> positions();
  sys::ptr<Buffer> texcoords();
  sys::ptr<Buffer> normals();

  bool intersect(const Ray& _ray, const mat4& _model, float& _distance) const;

  sys::vector<Face>::iterator faces_begin();
  sys::vector<Face>::iterator faces_end();

private:
  Buffer m_positions;
  Buffer m_texcoords;
  Buffer m_normals;

  sys::vector<Face> m_faces;
};

}

#endif

