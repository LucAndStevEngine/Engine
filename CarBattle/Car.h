#pragma once
#include "SceneNode.h"
#include "BulletDynamics\Vehicle\btRaycastVehicle.h"
#include <vector>

class Car : public SceneNode
{
public:
	Car();
	virtual ~Car();

	virtual void Init();
	virtual void Update();
	virtual void FixedUpdate();

protected:

	float m_YawSensitivity; 
	float m_EnginePower;
	float m_DownForce;
	float m_MaxwheelAngle;
	float m_Steering;
	AssimpModel* model;
	btRaycastVehicle::btVehicleTuning m_tuning;
	btVehicleRaycaster* m_vehicleRayCaster;
	btRaycastVehicle* m_vehicle;
	std::vector<SceneNode*> m_wheelNodes;

	float m_EngineForce;
	float m_BreakingForce;
	float m_MaxEngineForce;
	float m_MaxBreakingForce;

	float m_VehicleSteering;
	float m_SteeringIncrement;
	float m_SteeringClamp;
	float m_wheelRadius;
	float m_wheelWidth;
	float m_wheelFriction;
	float m_SuspensionStiffness;
	float m_SuspensionDamping;
	float m_SuspensionCompression;
	float m_RollInfluence;
	float m_SuspensionRestLength;

};

