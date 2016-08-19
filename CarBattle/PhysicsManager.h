#pragma once
#include "btBulletDynamicsCommon.h"
#include <vector>

class PhysicsManager
{
public:

	void Init();
	void Update();
	void Shutdown();

	PhysicsManager();
	~PhysicsManager();

	void SetGravity(btVector3* gravity);
	btVector3 GetGravity();

	void AddCollisionBody(class RigidBodyComponent* body);
	bool RemoveCollisionBody(class RigidBodyComponent* body);

private:
	btDefaultCollisionConfiguration* m_collisionConfig;
	btCollisionDispatcher* m_dispatcher;
	btBroadphaseInterface* m_overlappingPairCache;
	btSequentialImpulseConstraintSolver* m_solver;

	std::vector<class RigidBodyComponent*> m_rigidBodies;

	btDiscreteDynamicsWorld* m_dynamicWorld;

	btVector3 m_gravity;
	int m_maxTimeSteps;
};

