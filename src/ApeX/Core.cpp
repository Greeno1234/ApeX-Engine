#include <iostream>
#include "Core.h"
#include "Entity.h"
#include "Window.h"

namespace apex {



	std::shared_ptr<Core> Core::initialize()
	{
		std::shared_ptr<Core> rtn = std::make_shared<Core>();

		rtn->m_window = std::make_shared<Window>();
		rtn->m_self = rtn;
		//rtn->m_nativeWindow = SDL_CreateWindow();


		return rtn;
	}


	std::shared_ptr<Entity> Core::add_entity()
	{
		std::shared_ptr<Entity> rtn = std::make_shared<Entity>();

		
		rtn->m_core = m_self;
		rtn->m_self = rtn;

		m_entities.push_back(rtn);

		//std::cout << rtn->m_core.lock().get() << std::endl;   ///idk what this does

		return rtn;
	}


	void Core::start()
	{

		//m_running = true;
		//while m_running{.....}

		for (size_t i = 0; i < 25; i++)
		{
			for (size_t ei = 0; ei < m_entities.size(); ei++)
			{
				m_entities.at(ei)->tick();
			}
			std::cout << i << std::endl;

			for (size_t ei = 0; ei < m_entities.size(); ei++)
			{
				//if (!m_entities.alive())
				//{
				//
				//}
			}








		}
	}

	void Core::stop()
	{
		m_running = false;
	}

	std::shared_ptr<Window> Core::window() const
	{
		return m_window;
	}
}