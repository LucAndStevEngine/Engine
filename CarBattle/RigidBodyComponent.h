#pragma once
#include "Component.h"
#include "PhysicsManager.h"
#include "Transform.h"

#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

class RigidBodyComponent : public Component
{
public:
	Transform offset;
	btRigidBody* rigidBody;
	btCollisionShape* shape;

	btScalar mass;
	btScalar restitution;
	btScalar friction;
	btVector3 localInertia;

	RigidBodyComponent(float fMass = 1.0f, float fRestitution = 0.2f, float fFriction = 1.0f, glm::vec3 inertia = glm::vec3(1), btCollisionShape* cShape = new btBoxShape(btVector3(1, 1, 1)));
	~RigidBodyComponent();

	virtual void Init();
	virtual void Update();
	virtual void FixedUpdate();

	void UpdateSceneNode();
	void SetTransform(glm::vec3 pos, glm::quat rot);
	btTransform GetTransform();
};

