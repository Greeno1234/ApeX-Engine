#include "Component.h"

namespace apex
{
	struct Audio : Component
	{
		int on_initialise();
		

		void Play();

		void on_tick();

		void on_quit();
		
	private:

		
	};

}