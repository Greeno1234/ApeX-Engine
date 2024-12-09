#include "Resource.h"

namespace apex {


	struct Resource;


	struct Model : Resource
	{
		void onLoad();

	private:
		std::shared_ptr<rend::Model> m_model;
	};
}