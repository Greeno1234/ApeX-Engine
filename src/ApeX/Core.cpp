#include <iostream>
#include "Core.h"
#include "Entity.h"
#include "Window.h"
#include "Transform.h"
#include "Resources.h"

#include <rend/rend.h>

namespace apex {

	std::shared_ptr<Resources> Core::resources() const
	{
		return m_resources;
	}

	std::shared_ptr<Core> Core::initialize()
	{
		std::shared_ptr<Core> rtn = std::make_shared<Core>();

		rtn->m_window = std::make_shared<Window>();
		rtn->m_resources = std::make_shared<Resources>();
		rtn->m_self = rtn;
		//rtn->m_nativeWindow = SDL_CreateWindow();


		return rtn;
	}


	std::shared_ptr<Entity> Core::add_entity()
	{
		std::shared_ptr<Entity> rtn = std::make_shared<Entity>();

		
		rtn->m_core = m_self;
		rtn->m_self = rtn;

		rtn->m_transform = rtn->add_component<Transform>();

		m_entities.push_back(rtn);
		//std::cout << rtn->m_core.lock().get() << std::endl;   ///idk what this does

		return rtn;
	}


	void Core::start()
	{
		float x = 0;
		float y = 0;
		float z = -2;
		float angle = 0;
		glm::vec3 axis = { 0,1,0 };
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
				switch (evt.type)
				{
				case SDL_KEYDOWN:
					switch (evt.key.keysym.sym)
					{
					case SDLK_UP:
						y += 0.1f;
						z -= 0.2f;
						break;
					case SDLK_DOWN:
							y -= 0.1f;
							z += 0.2f;

						break;
					case SDLK_LEFT:
	
						std::cout<< "left";
						x -= 0.2f;
						
						break;
					case SDLK_RIGHT:

						std::cout << "right";
						x += 0.2f;
					
						break;

					case SDLK_d:

						std::cout << "d";
						angle += 10;
						break;
					case SDLK_a:

						std::cout << "a";
						angle -= 10;
						break;

					case SDLK_ESCAPE:
						stop();
						//m_running = false;
						break;
					default:
						break;
					}

				}
				m_entities[0]->get_transform()->setPosition(glm::vec3(x, y, z)); //player is entity 0
				m_entities[0]->get_transform()->setRotation(angle, glm::vec3(0, 1, 0));
				
			}

			for (size_t ei = 0; ei < m_entities.size(); ei++)
			{
				m_entities.at(ei)->tick();
			}

			SDL_Rend_ClearWindow(m_window->m_raw);

			for (size_t ei = 0; ei < m_entities.size(); ei++)
			{
				m_entities.at(ei)->display();
			}

			SDL_Rend_SwapWindow(m_window->m_raw);
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