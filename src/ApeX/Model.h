#include "Resource.h"

namespace apex {

	struct Resource;
	struct Renderer;

	struct Model : Resource
	{
		void onLoad();

	private:

		friend struct apex::Renderer;
		std::shared_ptr<rend::Model> m_model;
	};
}