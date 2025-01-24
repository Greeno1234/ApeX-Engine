#include <iostream>
#include "Core.h"
#include "Entity.h"
#include "Window.h"
#include "Transform.h"
#include "Keyboard.h"
#include "Camera.h"
#include "Resources.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <rend/rend.h>




namespace apex {


	std::shared_ptr<Resources> Core::resources() const
	{
		return m_resources;
	}

	std::shared_ptr<Keyboard> Core::getKeyboard()
	{
		return m_keyboard;
	}
	std::shared_ptr<Camera> Core::getCamera()
	{
		return m_camera;
	}

	std::shared_ptr<Core> Core::initialize()
	{
		std::shared_ptr<Core> rtn = std::make_shared<Core>();

		rtn->m_window = std::make_shared<Window>();
		rtn->m_resources = std::make_shared<Resources>();
		rtn->m_keyboard = std::make_shared<Keyboard>();
		rtn->m_camera = std::make_shared<Camera>();

		rtn->m_self = rtn;

		////////////////     Audio Initialisation    ////////////////
		ALCdevice* device = alcOpenDevice(NULL); ///< Playback device
		if (!device)
		{
			throw std::runtime_error("Failed to open audio device");
		}

		ALCcontext* context = alcCreateContext(device, NULL); ///< context of device (find out why this is important)
		if (!context)
		{
			alcCloseDevice(device);
			throw std::runtime_error("Failed to create audio context");
		}
		if (!alcMakeContextCurrent(context)) ///< checks if new context is current
		{
			alcDestroyContext(context); ///< destroys current device context
			alcCloseDevice(device);
			throw std::runtime_error("Failed to make context current");
		}
		alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f); ///< Listener (make this camera)
		//alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);


		return rtn;
	}


	std::shared_ptr<Entity> Core::add_entity()
	{
		std::shared_ptr<Entity> rtn = std::make_shared<Entity>();

		
		rtn->m_core = m_self;
		rtn->m_self = rtn;

		rtn->m_transform = rtn->add_component<Transform>(); //every entity will have its own transform component

		m_entities.push_back(rtn);
		//std::cout << rtn->m_core.lock().get() << std::endl;   ///idk what this does

		return rtn;
	}


	void Core::start() // main game loop
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
				switch (evt.type)
				{
				
				case SDL_KEYDOWN:
					m_keyboard->m_keys.push_back(evt.key.keysym.sym);///< gets key pressed and stores it in the m_keys vector
					break;
				case SDL_KEYUP:
					//clear the keys
					for (size_t i = 0; i < m_keyboard->m_keys.size(); ++i)
					{
						if (m_keyboard->m_keys[i] == evt.key.keysym.sym)
						{
							m_keyboard->m_keys.erase(m_keyboard->m_keys.begin() + i);
							--i;
						}
					}
					break;
				default:
					break;

				}
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

	Core::~Core() ///< destructor
	{
		alcMakeContextCurrent(NULL);
		//alcDestroyContext(context);
		//alcCloseDevice(device);
	}


	std::shared_ptr<Window> Core::window() const
	{
		return m_window;
	}
}