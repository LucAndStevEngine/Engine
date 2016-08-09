#include "SceneGraph.h"



SceneGraph::SceneGraph() 
{
	root = new SceneNode();
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
	delete root;
	root = new SceneNode();
}
