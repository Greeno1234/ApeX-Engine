#include "Model.h"

namespace apex {

	void Model::onLoad()
	{
		m_model = std::make_shared<rend::Model>(std::string(getPath() + ".obj").c_str()); //add .obj to end
	}

}