#ifndef REND_RENDSDL_H
#define REND_RENDSDL_H

#include "rendgl.h"
#include "Exception.h"

#include <SDL2/SDL.h>

#define SDL_WINDOW_REND SDL_WINDOW_OPENGL

typedef SDL_GLContext SDL_RendContext;

inline SDL_RendContext SDL_Rend_CreateContext(SDL_Window* _window)
{
  SDL_GLContext rtn = SDL_GL_CreateContext(_window);

  if(!rtn)
  {
    throw rend::Exception("Failed to initialize OpenGL");
  }

  if(glewInit() != GLEW_OK)
  {
    throw rend::Exception("Failed to initialize OpenGL extensions");
  }

  return rtn;
}

inline void SDL_Rend_DeleteContext(SDL_RendContext _context)
{
  SDL_GL_DeleteContext(_context);
}

inline void SDL_Rend_ClearWindow(SDL_Window* _window)
{
  int width = 0;
  int height = 0;
  SDL_GetWindowSize(_window, &width, &height);
  glViewport(0, 0, width, height);

  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

inline void SDL_Rend_SwapWindow(SDL_Window* _window)
{
  SDL_GL_SwapWindow(_window);
}

#endif

