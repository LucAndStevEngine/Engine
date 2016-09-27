#include "RenderComponent.h"
#include "Game.h"
#include "SceneNode.h"
#include "RenderingManager.h"

RenderComponent::RenderComponent()
{
}

RenderComponent::RenderComponent(ShaderProgram* shader)
{
	m_shader = shader;
}

RenderComponent::RenderComponent(ShaderProgram* shader, unsigned int VAO)
{
	m_shader = shader;
	m_VAO_ID = VAO;
}

void RenderComponent::SetShader(ShaderProgram* shader)
{
	m_shader = shader;
}

ShaderProgram* RenderComponent::GetShader()
{
	return m_shader;
}

void RenderComponent::SetVAO(unsigned int VAO)
{
	m_VAO_ID = VAO;
}

unsigned int RenderComponent::GetVAO()
{
	return m_VAO_ID;
}

void RenderComponent::Init()
{
	m_renderManager = sceneNode->GetGame()->GetRenderManager();
	m_renderManager->AddRenderComp(this);
}

void RenderComponent::Render()
{
}

RenderComponent::~RenderComponent()
{
}
