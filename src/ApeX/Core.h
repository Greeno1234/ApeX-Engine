#include <memory>
#include <vector>

namespace apex
{

	struct Entity;
	struct Window;

	struct Core
	{
		static std::shared_ptr<Core> initialize();
		
		void start();
		void stop();
		std::shared_ptr<Entity> add_entity();
		std::shared_ptr<Window> window() const;

	private:

		std::shared_ptr<Window> m_window;
		std::vector<std::shared_ptr<Entity> > m_entities;
		bool m_running = false;
		std::weak_ptr<Core> m_self;	
		//SDL_Window* m_nativeWindow;
	};

}