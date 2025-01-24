//#include <rend/rend.h>
#include "ApeX/apex.h" //find a way that doest just include the master header
//#include "Component.h"
//#include "Entity.h"

namespace apex
{

	struct BoxCollider : Component
	{
		bool colliding(const BoxCollider& _other);

	private:
		glm::vec3 m_size;
		glm::vec3 m_offset;

	};


}