#include "Transform.h"
#include <rend/rend.h>


namespace apex
{

	void Transform::on_initialize()
	{
		m_scale = glm::vec3(1, 1, 1);
	}
	
	glm::mat4 Transform::model()
	{
		
		glm::mat4 rtn(1.0f);

		rtn = glm::translate(rtn, m_position);
		//TODO: rotation
		rtn = glm::scale(rtn, m_scale);

		return rtn;
		
	}

	void Transform::setPosition(glm::vec3 pos)
	{
		m_position = pos;
	}

	void Transform::setScale(glm::vec3 scale)
	{
		m_scale = scale;
	}
	
}