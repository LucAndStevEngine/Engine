#include "RenderingManager.h"
#include "GL\glew.h"
#include "SceneNode.h"
#include "Camera.h"
#include "GLMath.h"
#include "ResourceManager.h"
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

	ShaderProgram* program = ResourceManager::GetShader("Text");
	if (program)
	{
		glDisable(GL_DEPTH_TEST);
		program->UseProgram();
		glm::mat4 proj = glm::ortho(0.0f, m_currentCamera->cameraWidth, 0.0f, m_currentCamera->cameraHeight);
		program->SetUniform("matrices.projMatrix", proj);
		for (int i = 0; i < m_textData.size(); i++)
		{
			program->SetUniform("vColor", m_textData[i]->color);
			m_textRenderer->Print(m_textData[i]->text, m_textData[i]->position.x, m_textData[i]->position.y, m_textData[i]->fontSize);
		}
		glEnable(GL_DEPTH_TEST);
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

void RenderingManager::InitText(std::string fontLocation, int pixelSize, int maxCharSupport)
{
	if (m_textRenderer)
	{
		return;
	}
	ShaderProgram* program = ResourceManager::GetShader("Text");
	if (!program)
	{
		Shader* shader = new Shader();
		shader->LoadShader("Shaders/TextFS.glsl", GL_FRAGMENT_SHADER);
		ResourceManager::SaveIndividualShader(shader, "TextFS");
		shader = new Shader();
		shader->LoadShader("Shaders/TextVS.glsl", GL_VERTEX_SHADER);
		ResourceManager::SaveIndividualShader(shader, "TextVS");

		program = new ShaderProgram();
		program->CreateProgram();
		program->AddShaderToProgram(ResourceManager::GetIndividualShader("TextFS"));
		program->AddShaderToProgram(ResourceManager::GetIndividualShader("TextVS"));
		program->LinkProgram();
		ResourceManager::SaveShader(program, "Text");
	}

	m_textRenderer = new TextRenderer();
	m_textRenderer->LoadFont(fontLocation, pixelSize, maxCharSupport);
	m_textRenderer->SetShaderProgram(program);
}

void RenderingManager::PrintText(std::string text, glm::vec4 color, glm::vec2 position, float fontSize, SceneNode* owner, std::string id)
{
	for (int i = 0; i < m_textData.size(); i++)
	{
		if (m_textData[i]->owner == owner && m_textData[i]->id == id)
		{
			m_textData[i]->text = text;
			m_textData[i]->color = color;
			m_textData[i]->position = position;
			m_textData[i]->fontSize = fontSize;
			return;
		}
	}

	TextData* data = new TextData();
	data->text = text;
	data->color = color;
	data->position = position;
	data->fontSize = fontSize;
	data->text = text;
	data->owner = owner;
	data->id = id;

	m_textData.push_back(data);
}

void RenderingManager::RemoveText(SceneNode* owner, std::string id)
{
	for (int i = 0; i < m_textData.size(); i++)
	{
		if (m_textData[i]->owner == owner && m_textData[i]->id == id)
		{
			delete m_textData[i];
			m_textData.erase(m_textData.begin() + i);
			return;
		}
	}
}
