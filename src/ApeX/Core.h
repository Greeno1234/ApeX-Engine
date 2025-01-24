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
	//struct Audio;

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

	private:

		std::shared_ptr<Window> m_window;
		std::shared_ptr<Resources> m_resources;
		std::shared_ptr<Keyboard> m_keyboard;
		std::shared_ptr<Camera> m_camera; 
		////sub systems
		// input
		// audio

		std::vector<std::shared_ptr<Entity> > m_entities;
		bool m_running = false;
		std::weak_ptr<Core> m_self;	
	};

	

}