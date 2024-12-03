#include "Component.h"
#include <rend/rend.h>


namespace apex
{


struct Transform : Component
{
	void on_initialize();

	glm::mat4 model(); //generates temporarily


	void setPosition(glm::vec3 pos);


private:
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;

};

}