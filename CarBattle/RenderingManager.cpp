#include "RenderingManager.h"
#include "GL\glew.h"
#include "SceneNode.h"
#include "Camera.h"
#include "GLMath.h"

unsigned int RenderingManager::m_currentVAO = -1;
ShaderProgram* RenderingManager::m_currentShader = NULL;
Camera* RenderingManager::m_currentCamera = NULL;

RenderingManager::RenderingManager()
{
	renderers.clear();
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

void RenderingManager::RenderScene(Camera* cam)
{ 
	m_currentCamera = cam;

	for (std::list<RenderComponent*>::iterator it = renderers.begin(); it != renderers.end(); it++)
	{
		RenderComponent* renderer = (RenderComponent*)*it;

		if (frustum.initialized && !renderer->bAlwaysDraw)
		{

			float biggestSide = renderer->sceneNode->transform.scale.x;
			if (renderer->sceneNode->transform.scale.y > biggestSide)
			{
				biggestSide = renderer->sceneNode->transform.scale.y;
			}
			if (renderer->sceneNode->transform.scale.z > biggestSide)
			{
				biggestSide = renderer->sceneNode->transform.scale.z;
			}
			bool draw = frustum.CheckSphere(renderer->sceneNode->transform.position, renderer->radius * biggestSide);
			if (!draw)
			{
				continue;
			}
		}

		if (renderer->GetShader() != m_currentShader)
		{
			m_currentShader = renderer->GetShader();
			m_currentShader->UseProgram();
			m_currentShader->SetUniform("projection", m_currentCamera->GetProjection());
			m_currentShader->SetUniform("view", m_currentCamera->LookAt());
		}
		if (renderer->GetVAO() != m_currentVAO)
		{
			m_currentVAO = renderer->GetVAO();
			glBindVertexArray(m_currentVAO);
		}

		renderer->Render();

	}
	m_currentShader = NULL;
	m_currentVAO = -1;
}

ShaderProgram * RenderingManager::GetCurrentShader()
{
	return m_currentShader;
}

unsigned int RenderingManager::GetCurrentVAO()
{
	return m_currentVAO;
}

Camera * RenderingManager::GetCurrentCamera()
{
	return m_currentCamera;
}
