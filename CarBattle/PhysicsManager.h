#pragma once
#include "btBulletDynamicsCommon.h"

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

	void AddCollisionShape(btRigidBody* shape);
	bool RemoveCollisionShape(btRigidBody* shape);

private:
	btDefaultCollisionConfiguration* m_collisionConfig;
	btCollisionDispatcher* m_dispatcher;
	btBroadphaseInterface* m_overlappingPairCache;
	btSequentialImpulseConstraintSolver* m_solver;

	btDiscreteDynamicsWorld* m_dynamicWorld;

	btVector3 m_gravity;
	int m_maxTimeSteps;
};

