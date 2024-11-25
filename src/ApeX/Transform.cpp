#include "Transform.h"
#include <glm/ext.hpp>


namespace apex
{
	
	glm::mat4 Transform::model()
	{
		
		glm::mat4 rtn(1.0f);

		rtn = glm::translate(rtn, m_position);
		//TODO: rotation
		rtn = glm::scale(rtn, m_scale);

		return rtn;
		
	};
	
}