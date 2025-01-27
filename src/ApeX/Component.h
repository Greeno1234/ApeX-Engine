#pragma once

#include <memory>



namespace apex
{
	struct Entity;

	struct Component
	{
		
		std::shared_ptr<Entity> entity();


	private:
		
		friend struct apex::Entity;
		virtual void on_initialize();
		virtual void on_tick();
		virtual void on_display();

		std::weak_ptr<Entity> m_entity;
		
		void tick();
		void display();
	};

}