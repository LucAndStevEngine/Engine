#pragma once

#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

#define PI 3.14
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

	static void QuaternionToEuler(const glm::quat &TQuat, glm::vec3 &TEuler);
};
