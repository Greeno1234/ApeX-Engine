#include "Component.h"

#include <rend/rend.h>

#include <memory>

namespace apex
{

	struct Texture;
	struct Model;
	struct Camera;

	struct Renderer : Component
	{

		void setTexture(std::shared_ptr<Texture> texture);
		void setModel(std::shared_ptr<Model> model);


		void on_initialize(); //virtual functions within component

		void on_display();


	private:
		//copy entire renderer, make it to a mesh renderer
		// 
		//turn these all into resources
		//wrap within resource

		std::shared_ptr<Model> m_model;
		std::shared_ptr<rend::Mesh> m_mesh; //do the same
		std::shared_ptr<Texture> m_texture;
		std::shared_ptr<rend::Shader> m_shader;
		std::shared_ptr<rend::ModelShader> m_mshader;

		std::shared_ptr<Camera> m_camera;


		glm::mat4 view;

	};


}