#pragma once


#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

class Plane
{
public:
	Plane();

	glm::vec3 normal;
	float d;

	void SetPoints(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
	float Distance(glm::vec3 p);
};

class FrustumCulling
{
public:
	FrustumCulling();

	void CreateFrustum(class Transform transform, float nearDist, float farDist, float ratio, float fov);
	bool CheckPoint(glm::vec3 pos);
	bool CheckSphere( glm::vec3 pos, float radius);
	~FrustumCulling();

	bool initialized;

private:
	Plane m_planes[6];
};


