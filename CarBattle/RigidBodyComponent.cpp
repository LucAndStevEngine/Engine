#include "RigidBodyComponent.h"
#include "SceneNode.h"
#include "Game.h"

RigidBodyComponent::RigidBodyComponent(float fMass, float fRestitution, float fFriction, glm::vec3 inertia, btCollisionShape* cShape) : mass(fMass), restitution(fRestitution), friction(fFriction), localInertia(inertia.x, inertia.y, inertia.z), shape(cShape)
{

}

RigidBodyComponent::~RigidBodyComponent()
{
	sceneNode->GetGame()->GetPhysicsManager()->RemoveCollisionBody(this);
	delete shape;
	delete rigidBody;
}

void RigidBodyComponent::Init()
{
	btTransform trans;
	trans.setIdentity();

	glm::vec3 pos = sceneNode->transform.position;
	trans.setOrigin(btVector3(pos.x, pos.y, pos.z));
	glm::quat rot = sceneNode->transform.rotation;
	trans.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));

	shape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(trans);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);
	rigidBody = new btRigidBody(rbInfo);

	sceneNode->GetGame()->GetPhysicsManager()->AddCollisionBody(this);

}

void RigidBodyComponent::Update()
{
}

void RigidBodyComponent::FixedUpdate()
{
}

void RigidBodyComponent::UpdateSceneNode()
{
	if (sceneNode != NULL)
	{
		btTransform trans = GetTransform();

		glm::vec3 position = glm::vec3(trans.getOrigin().x() - offset.position.x,
			trans.getOrigin().y() - offset.position.y,
			trans.getOrigin().z() - offset.position.z);


		sceneNode->transform.position = position;
		glm::vec3 euler;
		glm::quat rot = glm::quat(trans.getRotation().w(), trans.getRotation().x(), trans.getRotation().y(), trans.getRotation().z());

		Transform::QuaternionToEuler(rot, euler);

		std::cout << "Rigid Body | " << rot.x << " : " << rot.y << " : " << rot.z << " : " << rot.w << std::endl;
		std::cout << " SceneNode | " << sceneNode->transform.rotation.x << " : " << sceneNode->transform.rotation.y << " : " << sceneNode->transform.rotation.z << " : " << sceneNode->transform.rotation.w << std::endl;

		sceneNode->transform.euler = euler;
	}
}

void RigidBodyComponent::SetTransform(glm::vec3 pos, glm::quat rot)
{
	btTransform trans;
	trans.setIdentity();

	trans.setOrigin(btVector3(pos.x + offset.position.x, pos.y + offset.position.y, pos.z + offset.position.z));	
	btQuaternion rotation = btQuaternion(rot.x, rot.y, rot.z, rot.w);
	trans.setRotation(rotation);
	rigidBody->setWorldTransform(trans);
	
}

btTransform RigidBodyComponent::GetTransform()
{
	if (rigidBody != NULL)
		return rigidBody->getWorldTransform();
	else
		return btTransform();
	
}
