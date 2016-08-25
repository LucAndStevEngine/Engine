#include "Camera.h"

glm::mat4 Camera::LookAt()
{
	return glm::lookAt(transform.position, transform.position + transform.forward, transform.up);
}

glm::mat4 Camera::GetProjection()
{
	return m_projectionMat;
}

void Camera::SetProjection(glm::mat4 proj)
{
	m_projectionMat = proj;
}

Camera::Camera() 
{

}

Camera::~Camera()
{
}
