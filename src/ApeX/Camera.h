#include <rend/rend.h> 
#include "Component.h"

#include "GL/glew.h"
namespace apex
{

	struct Camera : Component
	{

		void setPosition(glm::vec3 pos)
		{
			cameraPos = pos;
		}
		void setPitch(float pitch)
		{
			cameraPitch = pitch;
		}
		void setYaw(float yaw)
		{
			cameraYaw = yaw;
		}

		float getYaw()
		{
			return cameraYaw;
		}

		glm::vec3 getDirection()
		{
			return direction;
		}

		void setTarget(glm::vec3 target)
		{
			cameraTarget = target;
		}

		glm::mat4 getView()
		{
			x = cos(glm::radians(cameraYaw)); //* cos(glm::radians(cameraPitch)) //removing this allows for consistent movement when looking up or down
			y = sin(glm::radians(cameraPitch));
			z = sin(glm::radians(cameraYaw)); //* cos(glm::radians(cameraPitch))
			direction = glm::vec3(x, y, z);
			cameraFront = glm::normalize(direction);
			return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		}

		
	private:

		float cameraYaw;
		float cameraPitch;
		float x;
		float y;
		float z;
		glm::vec3 cameraTarget;
		glm::vec3 direction = glm::vec3(x, y, z);
		
		glm::vec3 cameraPos;
		glm::vec3 cameraFront = glm::normalize(direction);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	};
}