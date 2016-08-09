#include "Camera.h"

glm::mat4 Camera::LookAt()
{
	return glm::lookAt(transform.position, transform.position + transform.forward, transform.up);
}

Camera::Camera() 
{

}

Camera::~Camera()
{
}
