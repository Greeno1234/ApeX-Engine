#include "Component.h"

namespace apex
{


	void Component::on_tick() {}
	void Component::on_display() {}
	void Component::on_initialize() {}

	std::shared_ptr<Entity> Component::entity() 
	{
		return m_entity.lock();
	}

	void Component::tick()
	{
		on_tick();
	}

	void Component::display()
	{
		on_display();
	}


}