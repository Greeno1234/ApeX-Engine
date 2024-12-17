#include "Renderer.h"
#include "Entity.h"
#include "Transform.h"
#include "Texture.h"
#include "Core.h"
#include "Resources.h"
#include "Model.h"



namespace apex {


	void Renderer::setTexture(std::shared_ptr<Texture> texture)
	{
		m_texture = texture;
	}
	void Renderer::setModel(std::shared_ptr<Model> model)
	{
		m_model = model;
	}



	void Renderer::on_initialize()
	{
		//m_shader = std::make_shared<rend::Shader>(rend::TEXTURE_SHADER); // create a shader

		m_mshader = std::make_shared<rend::ModelShader>();

		//m_mesh = std::make_shared<rend::Mesh>(rend::QUAD_MESH); // create a triangle mesh    /// this will be dynamically changeable

		//m_texture = entity()->core()->resources()->load<Texture>("textures/grass");
		
	}

	void Renderer::on_display()
	{
		/*if (!m_texture)
		{
			return;
		}*/



		glm::mat4 model(1.0f);
		
		model = entity()->get_transform()->model();


		m_mshader->model(*m_model->m_model);
		m_mshader->lighting(false);
		m_mshader->projection(glm::perspective(45.0f, 1.0f, 0.1f, 100.0f));
		m_mshader->model(model);

		m_mshader->depth_test(true);

		m_mshader->render();

	}



}