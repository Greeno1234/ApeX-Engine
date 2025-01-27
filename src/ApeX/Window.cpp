#include "Window.h"

#include <rend/rend.h>

namespace apex
{
	Window::Window()
	{
		int winw = 800;
		int winh = 600;

		m_raw = SDL_CreateWindow("Engine Demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, winw, winh, SDL_WINDOW_RESIZABLE | SDL_WINDOW_REND);

		if (!m_raw)
		{
			throw std::exception("Failed to create window");
		}

		m_context = SDL_Rend_CreateContext(m_raw);

	}

	Window::~Window()
	{
		SDL_Rend_DeleteContext(m_context);
		SDL_DestroyWindow(m_raw);
		SDL_Quit();
	}
}