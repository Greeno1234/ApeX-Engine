#include "Entity.h"
#include "Component.h"

namespace apex {

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
			//m_components.at(ci)->kill();
		}
	}

	std::shared_ptr<Transform> Entity::get_transform()
	{
		return m_transform.lock();
	}
	std::shared_ptr<AudioSource> Entity::get_audio()//
	{
		return m_audioSource.lock();
	}
}