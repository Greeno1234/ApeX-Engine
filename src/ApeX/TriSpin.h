#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <rend/rend.h>

#include <exception>
#include "Component.h"


namespace apex
{

	struct TriSpin : Component
	{
		bool quit;

		SDL_Window* window; //change to core
		SDL_Event e;

		void on_initialize();

		void on_render();

	};


}