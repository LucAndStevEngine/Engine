#pragma once
class Component
{
public:
	Component();

	class SceneNode* sceneNode;
	bool isEnable;
	virtual void Init();
	virtual void Update();
	virtual void FixedUpdate();

	virtual ~Component();
};

