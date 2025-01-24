#include <rend/rend.h>  ///might change this to just use necessary includes
//#include "ApeX/ApeX.h"
#include "Component.h"
namespace apex
{

	struct Camera : Component
	{
		/*
		glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, -3.0f);
		glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 cameraDirection = glm::normalize(cameraPosition - cameraDirection); // Z-axis vector
	
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);  
		glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection)); // X-axis vector

		glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);  // Y-axis vector

		// all of these vectors for the view/camera space

		*/
	


		//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 6.0f);
		glm::vec3 cameraPos;

		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


		void setPosition(glm::vec3 pos)
		{
			cameraPos = pos;
		}

		glm::mat4 getView()
		{
			return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		}

	};




}