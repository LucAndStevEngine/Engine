#pragma once

#include "SceneNode.h"

class Camera : public SceneNode
{
public:
	glm::mat4 LookAt();
	glm::mat4 GetProjection();
	void SetProjection(glm::mat4 proj);

	Camera();
	~Camera();

	float cameraWidth, cameraHeight;
private:
	glm::mat4 m_projectionMat;
};

