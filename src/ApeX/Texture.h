#include <GL/glew.h>
#include <string>
#include "Resource.h"

namespace apex {


	struct Resource;



	struct Texture : Resource
	{
		void onLoad();
	
	private:
		std::shared_ptr<rend::Texture> m_texture;
	};



}