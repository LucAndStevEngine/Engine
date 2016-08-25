#pragma once

#include "SceneNode.h"

class SceneGraph
{
public:
	SceneGraph(class Game* game);
	virtual ~SceneGraph();

	SceneNode* root;

	void Update();
	void FixedUpdate();
	void ClearGraph();
	SceneNode* MousePick(glm::vec3 start, glm::vec3 direct);
	void SetRoot(SceneNode* scenenode);
};


