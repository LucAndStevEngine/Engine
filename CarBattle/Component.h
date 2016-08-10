#pragma once
class Component
{
public:
	Component();

	class SceneNode* sceneNode;

	virtual void Init();
	virtual void Update();
	virtual void FixedUpdate();

	virtual ~Component();
};

