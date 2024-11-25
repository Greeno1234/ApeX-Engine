#include "rendmath.h"
#include "Buffer.h"
#include "Mesh.h"

#include <sys/memory.h>
#include <sys/linkedlist.h>
#include <sys/string.h>
#include <sys/vector.h>

namespace rend
{

struct Model;
struct Texture;

struct TextureInfo;

struct Material : sys::base
{
  Material();

  sys::string name;
  sys::ptr<Texture> texture;
  vec4 diffuse;
};

struct MaterialGroup : sys::base
{
  Mesh mesh;
  sys::ptr<Material> material;
};

struct Part : sys::base
{
  Part();

  sys::linked_list<MaterialGroup>::iterator mgs_begin();
  sys::linked_list<MaterialGroup>::iterator mgs_end();

  const sys::string& name() const;
  vec3 center() const;

  vec4 color() const;
  void color(const vec4& _color);

private:
  friend struct Model;

  sys::string m_name;
  sys::linked_list<MaterialGroup> m_mgs;
  vec3 m_center;
  vec3 m_size;
  vec4 m_color;
};

struct Model : sys::base
{
  Model();
  //Model(const Model& _copy);
  //Model& operator=(const Model& _assign);
  ~Model();

  Model(const sys::string& _path);
  void load(const sys::string& _path);
  Model(const sys::vector<unsigned char>& _data);
  void load(const sys::vector<unsigned char>& _data);

  sys::linked_list<Part>::iterator parts_begin();
  sys::linked_list<Part>::iterator parts_end();

  sys::linked_list<Material>::iterator materials_begin();
  sys::linked_list<Material>::iterator materials_end();

  vec3 size() const;
  int parts() const;

private:
  sys::linked_list<TextureInfo> m_textures;
  sys::linked_list<Material> m_materials;
  sys::linked_list<Part> m_parts;

  vec3 m_center;
  vec3 m_size;

  void process_obj(const sys::vector<sys::string>& _lines,
    const sys::string& _path = "");

  Vertex process_vertex(const sys::string& _token,
    const sys::vector<vec3>& _positions,
    const sys::vector<vec2>& _texcoords,
    const sys::vector<vec3>& _normals);

  void process_mtl(const sys::string& _path);
};

}

