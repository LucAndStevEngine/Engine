#include "Camera.h"



void Camera::Update()
{
	rot *= glm::quat(cos(rotation.z / 2), 0, 0, sin(rotation.z / 2) * 1);
	rot *= glm::quat(cos(rotation.x / 2), sin(rotation.x / 2) * 1, 0, 0);
	rot *= glm::quat(cos(rotation.y / 2), 0, sin(rotation.y / 2) * 1, 0);

	//currentTransform = glm::toMat4(rot) *  currentTransform;
	//currentTransform = glm::translate(temp, position) * currentTransform;

	upVector = glm::rotate(rot, m_upVectorOrigin);
	forwardVector = glm::rotate(rot, m_lookAtOrigin);

	rot = glm::quat();
}

glm::mat4 Camera::LookAt()
{
	return glm::lookAt(position, position + forwardVector, upVector);
}

Camera::Camera() : position(0, 0, 0), forwardVector(0, 0, 1), upVector(0, 1, 0)
{
	m_lookAtOrigin = forwardVector;
	m_upVectorOrigin = upVector;
}


Camera::~Camera()
{
}
