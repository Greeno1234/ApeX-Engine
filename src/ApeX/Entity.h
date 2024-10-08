#include <memory>
#include <vector>

namespace apex
{

	struct Core;
	struct Component;

	struct Entity
	{
		template <typename T>
		std::shared_ptr<T> add_component()
		{
			std::shared_ptr<T> rtn = std::make_shared<T>();

			m_components.push_back(rtn);

			return rtn;
		}

	private:

		friend struct apex::Core; //friend allows for private functions to be accessed by core struct

		std::weak_ptr<Core> m_core;
		std::vector<std::shared_ptr<Component> > m_components;



		//down the hierarchy use shared ptr
		//up the hierarchy use weak ptr
	};
}