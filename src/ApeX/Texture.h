#include <GL/glew.h>
#include <string>
#include "Resource.h"

namespace apex {
	struct Resource;
	struct TriangleRenderer;


	struct Texture : Resource
	{
		void onLoad();
	
	private:
		friend struct apex::TriangleRenderer;
		std::shared_ptr<rend::Texture> m_texture;
	};
}