#include "Texture.h"

namespace apex {


	void Texture::onLoad()
	{
		m_texture = std::make_shared<rend::Texture>(getPath().c_str()); //cant figure out how to add png to end
	}
}