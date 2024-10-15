#include <memory>

namespace apex
{


	struct Entity;

	struct Component
	{
		virtual void on_initialize();
		virtual void on_tick();


	private:
		
		friend struct apex::Entity;
		
		std::weak_ptr<Entity> m_entity;
		
		void tick();
		//int m_dummy;
	};

}