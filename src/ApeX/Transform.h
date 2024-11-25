#include "Component.h"
#include "glm/glm.hpp"



namespace apex
{


struct Transform : Component
{
	glm::mat4 model(); //generates temporarily
private:
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;

};

}