#include "Component.h"

#include <rend/rend.h>

#include <memory>

namespace apex
{

	struct Texture;
	struct Camera;


	struct TriangleRenderer : Component
	{
		
		void setTexture(std::shared_ptr<Texture> texture);


		void on_initialize(); //virtual functions within component

		void on_display();

		
	private:
		//copy entire renderer, make it to a mesh renderer
		// 
		//turn these all into resources
		//wrap within resource

		std::shared_ptr<rend::Mesh> m_mesh; //do the same
		std::shared_ptr<Texture> m_texture;
		std::shared_ptr<rend::Shader> m_shader;

		std::shared_ptr<Camera> m_camera;

		//Mesh m_mesh;


		/*

		difference is that mesh can be dynamically changed

		do the same thing to mesh as has been done to texture

		try to remove the rend:: starters 
		
		*/
	};


}