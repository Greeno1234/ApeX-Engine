#include "Texture.h"

namespace apex 
{
	void Texture::onLoad()
	{
		m_texture = std::make_shared<rend::Texture>(std::string(getPath() + ".png").c_str());
	}
}