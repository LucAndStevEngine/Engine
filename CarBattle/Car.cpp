#include "Car.h"
#include "btBulletCollisionCommon.h"
#include "RigidBodyComponent.h"
#include "Game.h"
#include <assert.h>
#include "glm\common.hpp"
#include "ResourceManager.h"
#include "ModelRenderer.h"
#define DELETE_NULL(x)      { if (x) delete x; x = NULL; }
#define CUBE_HALF_EXTENTS   1
Car::Car()
{
	m_Steering = 0.0f;
	m_EngineForce = 0.0f;
	m_BreakingForce = 0.0f;
	m_MaxEngineForce = 2500.0f;
	m_MaxBreakingForce = 100.f;
	m_VehicleSteering = 0.0f;
	m_SteeringIncrement = 0.04f;
	m_wheelFriction = 1000.0f;
	m_SuspensionStiffness = 14.0f;
	m_SuspensionDamping = 2.0f;
	m_SuspensionCompression = 4.0f;
	m_RollInfluence = 0.01f;
	m_SuspensionRestLength = 0.6f;
	m_vehicleRayCaster = NULL;
	m_vehicle = NULL;
	m_wheelNodes.clear();
}


Car::~Car()
{
	//DELETE_NULL(m_vehicleRayCaster);
	//DELETE_NULL(m_vehicle);
	//m_vpNodeWheel.Clear();
}

void Car::Init()
{
	//assert(GetParent() != NULL);
	RigidBodyComponent* Hull = new RigidBodyComponent(0, 10000.0f, 0.0f, glm::vec3(0), new btBoxShape(btVector3(1, 1, 1)));
	AddComponent(Hull);
	ModelRenderer* render = new ModelRenderer(ResourceManager::GetModel("models/BMW_M3_GTR/BMW_M3_GTR.obj"), ResourceManager::GetShader("Default"));
	AddComponent(render);
	
	Hull->mass = 800.0f;
	Hull->rigidBody->setDamping(0.2f, 0.5f);

	int rightIndex = 0;
	int upIndex = 1;
	int forwardIndex = 2;
	m_vehicleRayCaster = new btDefaultVehicleRaycaster(GetGame()->GetPhysicsManager()->getWorld());
	m_vehicle = new btRaycastVehicle(m_tuning, Hull->rigidBody, m_vehicleRayCaster);
	GetGame()->GetPhysicsManager()->AddVehicle(m_vehicle, Hull);
	m_vehicle->setCoordinateSystem(rightIndex, upIndex, forwardIndex);
	float connectionHeight = -0.4f;//1.2f;
	btVector3 wheelDirectionCS0(0, -1, 0);
	btVector3 wheelAxleCS(-1, 0, 0);

	bool isFrontWheel = true;

	btVector3 connectionPointCS0(CUBE_HALF_EXTENTS - (0.3f * m_wheelWidth), connectionHeight, 2 * CUBE_HALF_EXTENTS - m_wheelRadius);
	m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, m_SuspensionRestLength, m_wheelRadius, m_tuning, isFrontWheel);

	connectionPointCS0 = btVector3(-CUBE_HALF_EXTENTS + (0.3f* m_wheelWidth), connectionHeight, 2 * CUBE_HALF_EXTENTS - m_wheelRadius);
	m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, m_SuspensionRestLength, m_wheelRadius, m_tuning, isFrontWheel);

	isFrontWheel = false;

	connectionPointCS0 = btVector3(-CUBE_HALF_EXTENTS + (0.3f* m_wheelWidth), connectionHeight, -2 * CUBE_HALF_EXTENTS + m_wheelRadius);
	m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, m_SuspensionRestLength, m_wheelRadius, m_tuning, isFrontWheel);

	connectionPointCS0 = btVector3(CUBE_HALF_EXTENTS - (0.3f* m_wheelWidth), connectionHeight, -2 * CUBE_HALF_EXTENTS + m_wheelRadius);
	m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, m_SuspensionRestLength, m_wheelRadius, m_tuning, isFrontWheel);

	for (int i = 0; i < m_vehicle->getNumWheels(); i++)
	{
		btWheelInfo& wheel = m_vehicle->getWheelInfo(i);
		wheel.m_suspensionStiffness = m_SuspensionStiffness;
		wheel.m_wheelsDampingRelaxation = m_SuspensionDamping;
		wheel.m_wheelsDampingCompression = m_SuspensionCompression;
		wheel.m_frictionSlip = m_wheelFriction;
		wheel.m_rollInfluence = m_RollInfluence;
	}

	if (m_vehicle)
	{
		m_vehicle->resetSuspension();
		for (int i = 0; i < m_vehicle->getNumWheels(); i++)
		{
			m_vehicle->updateWheelTransform(i, true);
			btTransform transform = m_vehicle->getWheelTransformWS(i);
	//		glm::vec3 v3Origin = glm::vec3(transform.getOrigin().x, transform.getOrigin().y , transform.getOrigin().z);
		}
	}
}

void Car::Update()
{
}

void Car::FixedUpdate()
{
}
