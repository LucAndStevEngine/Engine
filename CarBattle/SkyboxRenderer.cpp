#include "SkyboxRenderer.h"
#include "Skybox.h"


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

	m_skybox->Render();

	glDepthMask(1);
}
