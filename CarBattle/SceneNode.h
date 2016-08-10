#pragma once
#include "Transform.h"
#include <vector>
#include "AssimpModel.h"

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
	std::vector<Component*> GetComponent();

	// Traverse the scene Graph through this function
	void TraverseGraph(void (SceneNode::*ptr)());

	class Game* GetGame();
protected:
	// Updates the current transform of the sceneNode
	void UpdateTransform();

	SceneNode* m_parent;
	std::vector<SceneNode*> m_children;
	class Game* m_game;
	std::vector<Component*> m_components;
	
private:
	//Origin look and upvector of the sceneNode
	glm::vec3 m_lookAtOrigin;
	glm::vec3 m_upVectorOrigin;
};

