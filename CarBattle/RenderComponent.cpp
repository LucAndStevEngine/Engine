#include "RenderComponent.h"



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


void RenderComponent::Render()
{
}

RenderComponent::~RenderComponent()
{
}
