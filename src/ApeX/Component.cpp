#include "Component.h"

namespace apex
{


	void Component::on_tick() {}
	void Component::on_display() {}
	void Component::on_initialize() {}


	void Component::tick()
	{
		on_tick();
	}

	void Component::display()
	{
		on_display();
	}


}