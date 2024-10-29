#include <SDL2/SDL.h>
#include <exception>


namespace apex
{

	struct Window
	{
		Window();
		~Window();

	private:
		SDL_Window* m_raw;
		//SDL_RendContext m_context;

		SDL_GLContext m_context;


		Window(const Window& _copy);
		Window& operator=(const Window& asign);
	};


}