#include <rend/rend.h>
#include "Component.h"


namespace apex
{
	struct BoxCollider : Component
	{
		bool colliding(BoxCollider& _other);

		void setSize(glm::vec3 size)
		{
			m_size = size;
		}
		

	private:
		glm::vec3 m_size;
		glm::vec3 m_offset;
	};


}