#include <memory>
#include <vector>
#include <stdexcept>

namespace apex
{
	struct Core;
	struct Component;
	struct Transform;
	struct AudioSource;

	struct Entity
	{
		void kill();


		template <typename T>
		std::shared_ptr<T> add_component()
		{
			std::shared_ptr<T> rtn = std::make_shared<T>();


			rtn->m_entity = m_self;
			rtn->on_initialize();
			m_components.push_back(rtn);

			return rtn;
		}

		template <typename T>
		std::shared_ptr<T> get_component()
		{
			for (size_t ci = 0; ci < m_components.size(); ++ci)
			{
				std::shared_ptr<T> rtn = std::dynamic_pointer_cast<T>(m_components.at(ci)); //expensive function (dpc)

				if (rtn) return rtn;
			}

			throw std::runtime_error("Failed to find component");
		}

		std::shared_ptr<Transform> get_transform();
		std::shared_ptr<AudioSource> get_audio();

		std::shared_ptr<Core> core();
	private:

		friend struct apex::Core; //friend allows for private functions to be accessed by core struct

		void tick();
		void display();

		std::vector<std::shared_ptr<Component>> m_components;
		std::weak_ptr<Transform> m_transform;
		std::weak_ptr<AudioSource> m_audioSource;
		bool m_alive = false;
		std::weak_ptr<Core> m_core;
		std::weak_ptr<Entity> m_self;
		//down the hierarchy use shared ptr
		//up the hierarchy use weak ptr
	};
}