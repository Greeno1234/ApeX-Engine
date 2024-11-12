#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <exception>
#include "Component.h"


namespace apex
{

	struct TriangleRenderer : Component
	{
		bool quit;
		
		
		void on_initialize(); //virtual functions?? within component

		void on_display();

		

		//Shader m_shader;
		//Mesh m_mesh;
	};


}