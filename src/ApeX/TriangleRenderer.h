#include "Component.h"

#include <rend/rend.h>

#include <memory>

namespace apex
{

	struct TriangleRenderer : Component
	{
		
		void on_initialize(); //virtual functions?? within component

		void on_display();

		
	private:
		//copy entire renderer, make it to a mesh renderer
		// 
		//turn these all into resources
		//wrap within resource

		std::shared_ptr<rend::Mesh> m_mesh;
		std::shared_ptr<rend::Texture> m_texture;
		std::shared_ptr<rend::Shader> m_shader;
		//Mesh m_mesh;
	};


}