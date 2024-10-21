#include <memory>

namespace apex
{


	struct Entity;

	struct Component
	{
		
		


	private:
		
		friend struct apex::Entity;
		virtual void on_initialize();
		virtual void on_tick();
		//virtual void onDisplay();


		std::weak_ptr<Entity> m_entity;
		
		void tick();
		//void display();
		//int m_dummy;
	};

}