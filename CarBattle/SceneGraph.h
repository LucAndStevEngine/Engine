#pragma once

#include "SceneNode.h"

class SceneGraph
{
public:
	SceneGraph();
	virtual ~SceneGraph();

	SceneNode* root;

	void Update();
	void FixedUpdate();
	void ClearGraph();
	
};


