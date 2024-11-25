#ifndef REND_RENDFLTK_H
#define REND_RENDFLTK_H

#include <FL/Fl_Gl_Window.H>

#include <sys/memory.h>

struct Fl_Rend_Window : Fl_Gl_Window, sys::base
{
  Fl_Rend_Window(int _x = 0, int _y = 0, int _w = 100, int _h = 100);

  rend::Ray ray_cast(int _x, int _y,
    const rend::mat4& _projection, const rend::mat4& _view) const;

  void clear();
  virtual void draw();

private:
  sys::zero<bool> m_initialized;
};

/* Implementation */

#include <GL/glew.h>

inline Fl_Rend_Window::Fl_Rend_Window(int _x, int _y, int _w, int _h)
  : Fl_Gl_Window(_x, _y, _w, _h)
{
  color(fl_rgb_color(240, 240, 240));
  end();
}

inline void Fl_Rend_Window::clear()
{
  glViewport(0, 0, w(), h());

  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

inline void Fl_Rend_Window::draw()
{
  if(!m_initialized)  
  {
    glewInit();
    m_initialized = true;
  }
}

inline rend::Ray Fl_Rend_Window::ray_cast(int _x, int _y,
  const rend::mat4& _projection, const rend::mat4& _view) const
{
  rend::Ray rtn;

  rend::vec4 viewport = rend::vec4(0, 0, w(), h());
  _y = (int)viewport.w - _y;

  rend::vec3 vnear = rend::unProject(rend::vec3(_x, _y, -1), _view, _projection, viewport);
  rend::vec3 vfar = rend::unProject(rend::vec3(_x, _y, 1), _view, _projection, viewport);

  rtn.direction = rend::normalize(vfar - vnear);
  rtn.origin = vnear;

  //mat4 invModel = rend::inverse(m_model);
  //rtn.origin = invModel * rend::vec4(rtn.origin, 1);
  //rtn.direction = invModel * rend::vec4(rtn.direction, 0);

  return rtn;
}

#endif

