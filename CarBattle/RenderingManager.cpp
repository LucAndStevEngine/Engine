#include "RenderingManager.h"
#include "RenderComponent.h"
#include "GL\glew.h"


unsigned int RenderingManager::m_currentVAO = -1;
ShaderProgram* RenderingManager::m_currentShader = NULL;

RenderingManager::RenderingManager()
{
}

void RenderingManager::AddRenderComp(RenderComponent* renderer)
{
	renderers.push_back(renderer);
}

void RenderingManager::RemoveRenderComp(RenderComponent* renderer)
{
	for (std::list<RenderComponent*>::iterator it = renderers.begin(); it != renderers.end(); it++)
	{
		RenderComponent* found = (RenderComponent*)*it;

		if (found == renderer)
		{
			renderers.erase(it);
			break;
		}
	}
}

void RenderingManager::RenderScene()
{ 

	for (std::list<RenderComponent*>::iterator it = renderers.begin(); it != renderers.end(); it++)
	{
		RenderComponent* renderer = (RenderComponent*)*it;

		if (renderer->GetShader() != m_currentShader)
		{
			m_currentShader = renderer->GetShader();
			m_currentShader->UseProgram();
		}
		if (renderer->GetVAO() != m_currentVAO)
		{
			m_currentVAO = renderer->GetVAO();
			glBindVertexArray(m_currentVAO);
		}

		renderer->Render();

	}
}
