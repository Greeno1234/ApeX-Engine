#include "Component.h"
#include <rend/rend.h>


namespace apex
{
	struct Transform : Component
	{
		void on_initialize();

		glm::mat4 model(); //generates temporarily

		glm::vec3 getPosition() { return m_position; };

		void setPosition(glm::vec3 position);
		void setScale(glm::vec3 scale);
		void setRotation(float angle, glm::vec3 axis);


	private:
		glm::vec3 m_position;
		float m_angle = 0.0f;
		glm::vec3 m_axis = glm::vec3 {0,1,0};
		glm::vec3 m_scale;

	};

}