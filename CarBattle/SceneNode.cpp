#include "SceneNode.h"
#include "Game.h"
#include "Component.h"
#include "RigidBodyComponent.h"
#include "Camera.h"
SceneNode::SceneNode()
{
	m_lookAtOrigin = transform.forward;
	m_upVectorOrigin = transform.up;
	Init();
}

SceneNode::~SceneNode()
{
	if (m_parent != NULL)
	{
		for (int i = 0; i < m_parent->m_children.size(); i++)
		{
			if (m_parent->m_children[i] == this)
			{
				m_parent->m_children.erase(m_parent->m_children.begin() + i);
			}		
		}
	}

	for (int i = 0; i < m_children.size(); i++)
	{
		delete m_children[i];
	}

	//for (int i = 0; i < m_components.size(); i++)
	//{
	//	delete m_components[i];
	//}
}


void SceneNode::AddChild(SceneNode* node)
{
	m_children.push_back(node);
	if (m_parent != NULL)
	{
		node->RemoveParent();
	}
	node->m_parent = this;
	node->m_game = m_game;
}

void SceneNode::RemoveParent()
{
	if (m_parent != NULL)
	{
		m_parent->RemoveChild(this);
		m_parent = NULL;
	}
}

void SceneNode::SetParent(SceneNode* node)
{
	node->AddChild(this);
	m_game = node->m_game;
}

void SceneNode::RemoveChild(SceneNode* node)
{
	for (int i = 0; i < m_children.size(); i++)
	{
		if (m_children[i] == node)
		{
			m_children[i] = NULL;
			m_children.erase(m_children.begin() + i);
			node->RemoveParent();		
			break;
		}
	}
}

std::vector<SceneNode*> SceneNode::GetChildren()
{
	return m_children;
}


SceneNode* SceneNode::GetParent()
{
	return m_parent;
}

void SceneNode::AddComponent(Component *comp)
{
	m_components.push_back(comp);
	comp->sceneNode = this;
	comp->Init();
}

void SceneNode::RemoveComponent(Component *comp)
{
	for (int i = 0; i < m_components.size(); i++)
	{
		if (m_components[i] == comp)
		{
			m_components[i] = NULL;
			m_components.erase(m_components.begin() + i);
			break;
		}
	}
}

std::vector<Component*> SceneNode::GetComponent()
{
	return m_components;
}

PickingInfo SceneNode::CheckPointCollision(glm::vec3 start, glm::vec3 direct)
{
	PickingInfo temp;
	float distance = glm::distance(transform.position,start);
	temp.distance = distance;
	temp.hitNode = this;

	glm::vec3 point = start + direct * distance;
	//std::cout << point.x << "  " << point.y << "  " << point.z << std::endl;
	if (glm::distance(this->transform.position, point) < 1)
		temp.hit = true;
	else
		temp.hit = false;
	return temp;
}

void SceneNode::TraverseGraph(void(SceneNode::*ptr)())
{
	(this->*ptr)();
	// use of function pointers so i dont have to rewrite the traverse logic
	if (m_children.size() > 0)
	{
		for (int i = 0; i < m_children.size(); i++)
		{
			(m_children[i]->TraverseGraph(ptr));
		}
	}
}

PickingInfo  SceneNode::TraverseGraph(PickingInfo(SceneNode::* ptr)(glm::vec3 s, glm::vec3 d),glm::vec3 start, glm::vec3 direct)
{
	PickingInfo temp;
	if (this->GetParent() != NULL && (dynamic_cast<Camera*>(this)) == NULL)
	{
		temp = (this->*ptr)(start, direct);
	}
	PickingInfo secondTemp;
	for (int i = 0; i < m_children.size(); i++)
	{
		secondTemp = m_children[i]->TraverseGraph(ptr, start, direct);

		if ( temp.hit && secondTemp.hit)
		{
			if (temp.distance > secondTemp.distance)
				temp = secondTemp;
		}
		else if (secondTemp.hit)
		{
			temp = secondTemp;
		}
	}
	return temp;
}

void SceneNode::AddRigidBody(RigidBodyComponent* rigidBody)
{
	m_rigidBody = rigidBody;
	AddComponent(rigidBody);
}

void SceneNode::RemoveRigidBody()
{
	RemoveComponent(m_rigidBody);
	delete m_rigidBody;
	m_rigidBody = NULL;
}


void SceneNode::Init()
{
	for (int i = 0; i < m_components.size(); i++)
	{
		m_components[i]->Init();
	}
	UpdateTransform();
}

void SceneNode::Update() 
{
	for (int i = 0; i < m_components.size(); i++)
	{
		m_components[i]->Update();
	}

	UpdateTransform();
}

void SceneNode::FixedUpdate()
{
	for (int i = 0; i < m_components.size(); i++)
	{
		m_components[i]->FixedUpdate();
	}
	UpdateTransform();
}

void SceneNode::UpdateTransform()
{
	glm::mat4 identity;
	glm::mat4 currentTransform;

	glm::quat rot;

	float degreeToRadian = 3.14 / 180;

	rot *= glm::quat(cos(transform.euler.z * degreeToRadian / 2), 0, 0, sin(transform.euler.z * degreeToRadian / 2) * 1);
	rot *= glm::quat(cos(transform.euler.x * degreeToRadian / 2), sin(transform.euler.x * degreeToRadian / 2) * 1, 0, 0);
	rot *= glm::quat(cos(transform.euler.y * degreeToRadian / 2), 0, sin(transform.euler.y * degreeToRadian / 2) * 1, 0);

	transform.rotation = rot;

	transform.up = glm::rotate(transform.rotation, m_upVectorOrigin);
	transform.forward = glm::rotate(transform.rotation, m_lookAtOrigin);

	currentTransform = glm::scale(identity, transform.scale) * currentTransform;
	currentTransform = glm::toMat4(rot) * currentTransform;
	currentTransform = glm::translate(identity, transform.position) * currentTransform;
	
	if (m_parent != NULL)
	{
		currentTransform = m_parent->transform.model * currentTransform;
	}

	transform.model = currentTransform;
	
	if (m_rigidBody != NULL)
	{
		m_rigidBody->SetTransform(transform.position, transform.rotation);
	}
}


Game* SceneNode::GetGame()
{
	return m_game;
}


/// Luc Vo - "Better than Firas!" - 4:00P.M August, Tuesday 16, 2016