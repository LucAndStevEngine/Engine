#pragma once
#include "Transform.h"
#include <vector>
#include "AssimpModel.h"

class SceneNode
{
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

	// Traverse the scene Graph through this function
	void TraverseGraph(void (SceneNode::*ptr)());
		
protected:
	// Updates the current transform of the sceneNode
	void UpdateTransform();

	SceneNode* m_parent;
	std::vector<SceneNode*> m_children;

	//std::vector<class Component*> m_components;
	
private:
	//Origin look and upvector of the sceneNode
	glm::vec3 m_lookAtOrigin;
	glm::vec3 m_upVectorOrigin;
};

