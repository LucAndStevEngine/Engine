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
