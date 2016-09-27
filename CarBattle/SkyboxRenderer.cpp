#include "SkyboxRenderer.h"
#include "Skybox.h"
#include "RenderingManager.h"
#include "Camera.h"

SkyboxRenderer::SkyboxRenderer()
{
	bAlwaysDraw = true;
}


SkyboxRenderer::~SkyboxRenderer()
{
}

void SkyboxRenderer::Render()
{
	glDepthMask(0);

	m_renderManager->GetCurrentShader()->SetUniform("model", glm::mat4());
	m_renderManager->GetCurrentShader()->SetUniform("color", glm::vec4(1, 1, 1, 1));
	m_renderManager->GetCurrentShader()->SetUniform("view", glm::translate(m_renderManager->GetCurrentCamera()->LookAt(), m_renderManager->GetCurrentCamera()->transform.position));
	m_skybox->Render();

	glDepthMask(1);
}
