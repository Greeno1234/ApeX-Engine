#include <iostream>
#include "Core.h"
#include "Entity.h"

namespace apex {



	std::shared_ptr<Core> Core::initialize()
	{
		std::shared_ptr<Core> rtn = std::make_shared<Core>();

		rtn->m_self = rtn;
		//rtn->m_nativeWindow = SDL_CreateWindow("OpenGL Demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);


		return rtn;
	}


	std::shared_ptr<Entity> Core::add_entity()
	{
		std::shared_ptr<Entity> rtn = std::make_shared<Entity>();

		
		rtn->m_core = m_self;
		rtn->m_self = rtn;

		m_entities.push_back(rtn);

		//std::cout << rtn->m_core.lock().get() << std::endl;   ///checks that it isnt empty

		return rtn;
	}


	void Core::start()
	{

		m_running = true;
		while (m_running)
		{

			for (size_t ei = 0; ei < m_entities.size(); ei++)
			{
				m_entities.at(ei)->tick();
			}
			for (size_t ei = 0; ei < m_entities.size(); ei++)
			{
				//if (!m_entities.alive())
				//{
				//
				//}
				m_entities.at(ei)->render();

			}
		}
	}

	void Core::stop()
	{
		m_running = false;
	}

}