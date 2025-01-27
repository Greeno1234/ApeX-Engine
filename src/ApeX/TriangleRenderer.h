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
		std::shared_ptr<rend::Mesh> m_mesh;
		std::shared_ptr<Texture> m_texture;
		std::shared_ptr<rend::Shader> m_shader;

		std::shared_ptr<Camera> m_camera;
	};


}