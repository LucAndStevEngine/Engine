#include "SceneGraph.h"
#include "Game.h"


SceneGraph::SceneGraph(Game* game)
{
	root = new SceneNode();
	root->m_game = game;
}


SceneGraph::~SceneGraph()
{
	delete root;
}

void SceneGraph::Update()
{
	root->TraverseGraph(&SceneNode::Update);
}

void SceneGraph::FixedUpdate()
{
	root->TraverseGraph(&SceneNode::FixedUpdate);
}

void SceneGraph::ClearGraph()
{
	Game* game = root->m_game;
	delete root;
	root = new SceneNode();
	root->m_game = game;
}

SceneNode* SceneGraph::MousePick(glm::vec3 start, glm::vec3 direct)
{
	PickingInfo temp = root->TraverseGraph(&SceneNode::CheckPointCollision, start, direct);
	if (temp.hit)
		return temp.hitNode;
	return NULL;
}

void SceneGraph::SetRoot(SceneNode * scenenode)
{
	Game* temp = root->m_game;
	delete root;
	root = scenenode;
	root->m_game = temp;
}
