#pragma once

#include "SceneNode.h"

class Camera : public SceneNode
{
public:
	glm::mat4 LookAt();

	Camera();
	~Camera();
};

