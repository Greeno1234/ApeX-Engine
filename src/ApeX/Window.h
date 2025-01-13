#include <SDL2/SDL.h>
#include <exception>


namespace apex
{
	struct Core;

	struct Window
	{
		Window();
		~Window();

	private:
		friend struct apex::Core;

		SDL_Window* m_raw;
		SDL_GLContext m_context;


		Window(const Window& _copy);
		Window& operator=(const Window& asign);
	};


}