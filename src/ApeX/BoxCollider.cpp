#include "BoxCollider.h"



namespace apex 
{
	bool BoxCollider::colliding(const BoxCollider& _other)
	{
		glm::vec3 a = entity()->get_transform()->getPosition();
		//glm::vec3 b = _other.entity()->get_transform()->getPosition();
		glm::vec3 b = entity()->get_transform()->getPosition();
		glm::vec3 ahs = m_size / 2.0f;
		glm::vec3 bhs = _other.m_size / 2.0f;

		// x
		if (a.x > b.x)
		{
			if (b.x + bhs.x < a.x - ahs.x)
			{
				return false;
			}
		}
		else
		{
			if (a.x + ahs.x < b.x - bhs.x) 
			{
				return false;
			}
		}
		// y
		if (a.y > b.y)
		{
			if (b.y + bhs.y < a.y - ahs.y)
			{
				return false;
			}
		}
		else
		{
			if (a.y + ahs.y < b.y - bhs.y)
			{
				return false;
			}
		}
		// z
		if (a.z > b.z)
		{
			if (b.z + bhs.z < a.z - ahs.z)
			{
				return false;
			}
		}
		else
		{
			if (a.z + ahs.z < b.z - bhs.z)
			{
				return false;
			}
		}

		return true;
	}


}