#include "TriangleRenderer.h"
#include "Entity.h"
#include "Transform.h"



namespace apex {
	void TriangleRenderer::on_initialize()
	{		
		m_shader = std::make_shared<rend::Shader>(rend::TEXTURE_SHADER);
		m_mesh = std::make_shared<rend::Mesh>(rend::TRIANGLE_MESH);
		m_texture = std::make_shared<rend::Texture>("../resources/textures/sample.png");
	}

	void TriangleRenderer::on_display()
	{

		
		glm::mat4 model(1.0f);
		//model = glm::translate(model, glm::vec3(0,0,-10));
		//model = entity()->get_component<Transform>()->model();
		model = entity()->get_transform()->model();

		m_shader->attribute("a_Position", *m_mesh->positions());
		m_shader->attribute("a_TexCoord", *m_mesh->texcoords());

		m_shader->uniform("u_Projection", glm::perspective(45.0f, 1.0f, 0.1f, 100.0f));
		m_shader->uniform("u_Model", model);
		m_shader->uniform("u_Texture", *m_texture);

		m_shader->render();

			/////////////////////////////
			//m_shader.set_mesh(m_mesh)//
			//m_shader.draw();         //
			/////////////////////////////

	}

}