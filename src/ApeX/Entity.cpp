#include "Entity.h"
#include "Component.h"

namespace apex {

	/*
	template <typename T>
	std::shared_ptr<T> Entity::add_component()
	{
		std::shared_ptr<T> rtn = std::make_shared<T>();


		rtn->m_entity = m_self;
		rtn->on_initialize();
		m_components.push_back(rtn);

		return rtn;
	} 
	*/

	std::shared_ptr<Core> Entity::core()
	{
		return m_core.lock();
	}


	void Entity::tick()
	{
		for (size_t ci = 0; ci < m_components.size(); ci++)
		{
			m_components.at(ci)->tick();
		}
	}

	void Entity::display()
	{
		for (size_t ci = 0; ci < m_components.size(); ci++)
		{
			m_components.at(ci)->display();
		}
	}

	void Entity::kill()
	{
		if (!m_alive) return;

		m_alive = false;

		for (size_t ci = 0; ci < m_components.size(); ci++)
		{
			//m_components.at(ci)->kill();   /////////////////not working with entity::kill either
		}
	}

	std::shared_ptr<Transform> Entity::get_transform()
	{
		return m_transform.lock();
	}
}