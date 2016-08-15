#include "FrustumCulling.h"
#include "Transform.h"
#include <iostream>

#define ANG2RAD 3.14159265358979323846/180.0

FrustumCulling::FrustumCulling()
{
	initialized = false;
}

void FrustumCulling::CreateFrustum(Transform camTransform, float nearDist, float farDist, float ratio, float fov)
{
	float tang = (float)tan(ANG2RAD * fov * 0.5f);

	float nearHeight = tang * nearDist;
	float nearWidth = nearHeight * ratio;

	float farHeight = tang * farDist;
	float farWidth = farHeight * ratio;

	glm::vec3 dir, nc, fc, x, y, z;

	z = -camTransform.forward;
	z = glm::normalize(z);

	x = glm::cross(camTransform.up, z);
	x = glm::normalize(x);

	y = glm::cross(z, x);

	nc = camTransform.position - z * nearDist;
	fc = camTransform.position - z * farDist;


	glm::vec3 nlb, nlt, nrb, nrt;
	glm::vec3 flb, flt, frb, frt;

	//Calculate far plane
	flt = fc + y * farHeight - x * farWidth;
	flb = fc - y * farHeight - x * farWidth;
	frt = fc + y * farHeight + x * farWidth;
	frb = fc - y * farHeight + x * farWidth;

	//Calculate near plane
	nlt = nc + y * nearHeight - x * nearWidth;
	nlb = nc - y * nearHeight - x * nearWidth;
	nrt = nc + y * nearHeight + x * nearWidth;
	nrb = nc - y * nearHeight + x * nearWidth;

	m_planes[0].SetPoints(nrt, nlt, flt);
	m_planes[1].SetPoints(nlb, nrb, frb);
	m_planes[2].SetPoints(nlt, nlb, flb);
	m_planes[3].SetPoints(nrb, nrt, frb);
	m_planes[4].SetPoints(nlt, nrt, nrb);
	m_planes[5].SetPoints(frt, flt, flb);

	initialized = true;
}

bool FrustumCulling::CheckPoint(glm::vec3 pos)
{
	for (int i = 0; i < 6; i++)
	{
		float val = m_planes[i].Distance(pos);
		if (val < 0)
		{
			return false;
		}
	}
	return true;
}

bool FrustumCulling::CheckSphere(glm::vec3 pos, float radius)
{
	for (int i = 0; i < 6; i++)
	{
		float val = m_planes[i].Distance(pos);
		if (val < -radius)
		{
			return false;
		}
	}
	return true;
}


FrustumCulling::~FrustumCulling()
{
}

Plane::Plane()
{

}

void Plane::SetPoints(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
{
	glm::vec3 aux1, aux2;
	aux1 = v1 - v2;
	aux2 = v3 - v2;

	normal = glm::cross(aux2, aux1);

	normal = glm::normalize(normal);
	d = -glm::dot(normal, v2);
}

float Plane::Distance(glm::vec3 p)
{
	return (d + glm::dot(normal, p));
}