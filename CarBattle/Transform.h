#pragma once

#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

class Transform
{
public:
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 euler;

	glm::vec3 forward;
	glm::vec3 up;

	glm::mat4 model;
	glm::quat rotation;

	Transform();
};
