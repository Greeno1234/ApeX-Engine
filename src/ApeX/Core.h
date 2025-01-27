#include <memory>
#include <vector>
#include <string>

namespace apex
{
	struct Entity;
	struct Window;
	struct Resources;
	struct Keyboard;
	struct Camera;
	struct BoxCollider;

	struct Core
	{
		static std::shared_ptr<Core> initialize();
		
		void start();
		void stop();
		Core::~Core();
		std::shared_ptr<Entity> add_entity();
		std::shared_ptr<Window> window() const;
		std::shared_ptr<Resources> resources() const;
		std::shared_ptr<Keyboard> getKeyboard();
		std::shared_ptr<Camera> getCamera();

		// find an entity with boxcolider
		template <typename T>
		void find(std::vector<std::shared_ptr<T>> & _out)
		{
			for (size_t ei = 0; ei < m_entities.size(); ei++) //< go through each Entity in Core
			{

				std::shared_ptr<Entity> e = m_entities.at(ei);

				for (size_t ci = 0; ci < e->m_components.size(); ci++) //< go through each Component in Entity
				{
					std::shared_ptr<Component> c = e->m_components.at(ci);

					std::shared_ptr<T> t = std::dynamic_pointer_cast<T>(c); //< try to dynamic cast the Component to T
					
					if (t)
					{
						_out.push_back(t);
					}
				}

			}
		}

	private:

		std::shared_ptr<Window> m_window;
		std::shared_ptr<Resources> m_resources;
		std::shared_ptr<Keyboard> m_keyboard;
		std::shared_ptr<Camera> m_camera; 

		std::vector<std::shared_ptr<Entity> > m_entities;
		std::weak_ptr<Core> m_self;
		bool m_running = false;
	};

	

}