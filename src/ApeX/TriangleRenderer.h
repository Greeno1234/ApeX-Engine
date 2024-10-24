#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <exception>
#include "Component.h"



namespace apex
{

	struct TriangleRenderer : Component
	{
		bool quit;

		SDL_Window* window; //change to core
		SDL_Event e;

		void on_initialize(); //virtual functions?? within component

		void on_render();

		//Shader m_shader;
		//Mesh m_mesh;
	};


}