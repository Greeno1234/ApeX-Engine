#include "Component.h"

namespace apex
{


	void Component::on_tick() {}
	void Component::on_initialize() {}
	void Component::on_render() {}


	void Component::tick()
	{
		on_tick();
	}

	void Component::render()
	{
		on_render();
	}

}