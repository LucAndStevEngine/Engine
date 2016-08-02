#pragma once

#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

class Camera
{
public:
	glm::vec3 position;
	glm::vec3 rotation;

	glm::vec3 forwardVector;
	glm::vec3 upVector;
	
	glm::mat4 temp;
	glm::mat4 currentTransform;
	glm::quat rot;

	void Update();

	glm::mat4 LookAt();

	Camera();
	~Camera();
private:
	glm::vec3 m_lookAtOrigin;
	glm::vec3 m_upVectorOrigin;
};

