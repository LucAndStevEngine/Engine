#include "PhysicsManager.h"
#include "Time.h"

PhysicsManager::PhysicsManager()
{
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	m_collisionConfig = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	m_dispatcher = new btCollisionDispatcher(m_collisionConfig);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	m_overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	m_solver = new btSequentialImpulseConstraintSolver();

	m_dynamicWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_overlappingPairCache, m_solver, m_collisionConfig);

}


PhysicsManager::~PhysicsManager()
{
}

void PhysicsManager::Init()
{
	m_maxTimeSteps = 10;
	m_dynamicWorld->setGravity(m_gravity);
}

void PhysicsManager::Update()
{
	m_dynamicWorld->stepSimulation(Time::fixedDeltaTime, m_maxTimeSteps);
}

void PhysicsManager::Shutdown()
{
	for (int i = m_dynamicWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = m_dynamicWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		m_dynamicWorld->removeCollisionObject(obj);
	}

	delete m_dynamicWorld;
	delete m_solver;
	delete m_overlappingPairCache;
	delete m_dispatcher;
	delete m_collisionConfig;
}

void PhysicsManager::SetGravity(btVector3* gravity)
{
	m_gravity = *gravity;
	m_dynamicWorld->setGravity(m_gravity);
}

btVector3 PhysicsManager::GetGravity()
{
	return m_gravity;
}

void PhysicsManager::AddCollisionShape(btRigidBody* shape)
{
	m_dynamicWorld->addRigidBody(shape);
}

bool PhysicsManager::RemoveCollisionShape(btRigidBody* shape)
{
	m_dynamicWorld->removeRigidBody(shape);
	return true;
}



