#pragma once
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
class RayCast
{
public:
	RayCast();
	~RayCast();
	static  glm::vec3 GetRayCastDirection(class Camera* camera, int posX, int posY, int width, int height);
};

