#include <iostream>
#include "Core.h"
#include "Entity.h"
#include "Window.h"

#include <GL/glew.h>

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

		m_running = true;
		while (m_running)
		{
			SDL_Event evt = {};

			while (SDL_PollEvent(&evt))
			{
				if (evt.type == SDL_QUIT)
				{
					m_running = false;
				}
			}

			for (size_t ei = 0; ei < m_entities.size(); ei++)
			{
				m_entities.at(ei)->tick();
			}

			glClearColor(0, 0, 0.5f, 1); //background colour
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			for (size_t ei = 0; ei < m_entities.size(); ei++)
			{
				m_entities.at(ei)->display();
			}

			SDL_GL_SwapWindow(m_window->m_raw);
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