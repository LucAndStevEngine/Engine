#pragma once
#include "Transform.h"
#include <vector>
#include "AssimpModel.h"

struct PickingInfo
{
	PickingInfo() : hitNode(NULL), distance(0), hit(false)
	{
	
	}
	class SceneNode* hitNode;
	float distance;
	bool hit;
};

class Component;

class SceneNode
{
	friend class SceneGraph;
public:

	// the current transform of the object
	Transform transform;

	SceneNode();
	virtual ~SceneNode();

	//Initialization for the object
	virtual void Init();

	// Updates for the sceneNodes
	virtual void Update();
	virtual void FixedUpdate();

	// Children management
	void AddChild(SceneNode* node);
	void RemoveChild(SceneNode* node);
	std::vector<SceneNode*> GetChildren();

	// Parent management
	void SetParent(SceneNode* node);
	void RemoveParent();
	SceneNode* GetParent();

	// Component Management
	void AddComponent(Component* node);
	void RemoveComponent(Component* node);
	std::vector<Component*> GetComponents();
	template<class T>
	T* GetComponent();
	PickingInfo CheckPointCollision(glm::vec3 start, glm::vec3 direct);
	// Traverse the scene Graph through this function
	void TraverseGraph(void (SceneNode::*ptr)());
	PickingInfo TraverseGraph(PickingInfo(SceneNode::* ptr)(glm::vec3 s, glm::vec3 d), glm::vec3 start, glm::vec3 direct);
	SceneNode TraverseGraph(void (SceneGraph::*ptr)());

	void AddRigidBody(class RigidBodyComponent* rigidBody);
	void RemoveRigidBody();

	class Game* GetGame();
protected:
	// Updates the current transform of the sceneNode
	void UpdateTransform();

	SceneNode* m_parent;
	std::vector<SceneNode*> m_children;
	class Game* m_game;
	std::vector<Component*> m_components;
	class RigidBodyComponent* m_rigidBody;
	
private:
	//Origin look and upvector of the sceneNode
	glm::vec3 m_lookAtOrigin;
	glm::vec3 m_upVectorOrigin;
};


template<class T>
inline T * SceneNode::GetComponent()
{
	for (int i = 0; i < m_components.size(); i++)
	{
		T* t = dynamic_cast<T*>(m_components[i]);
			if (t != NULL)
				return t;
	}
	return NULL;
}
