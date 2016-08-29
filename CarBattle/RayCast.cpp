#include "RayCast.h"
#include "Camera.h"
#include "SceneNode.h"
#include "WindowControl.h"


glm::vec3 RayCast::GetRayCastDirection(Camera * camera, int posX, int posY, int width, int height)
{
	float x = (2.0f * posX) / width - 1.0f;
	float y = 1.0f - (2.0f * posY) / height;
	float z = 1.0f;

	glm::vec3 ray_nds = glm::vec3(x, y, z);
	glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);
	glm::vec4 ray_eye = inverse(camera->GetProjection()) * ray_clip;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
    glm::vec4 temp = (inverse(camera->LookAt()) * ray_eye);
	glm::vec3 ray_wor = glm::vec3(temp.x, temp.y, temp.z);
	ray_wor = glm::normalize(ray_wor);
	//ray_wor.x *= -1;
	//std::cout << ray_wor.x << "   " << ray_wor.y << "  " << ray_wor.z << std::endl;
	return ray_wor;
}

RayCast::RayCast()
{
}


RayCast::~RayCast()
{
}
