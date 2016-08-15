#pragma once

#include "Shader.h"
#include <list>
#include "RenderComponent.h"
#include "FrustumCulling.h"

class RenderingManager
{
public:
	RenderingManager();

	void AddRenderComp(RenderComponent* renderer);
	void RemoveRenderComp(RenderComponent* renderer);

	void RenderScene();

	std::list<RenderComponent*> renderers;

	FrustumCulling frustum;
protected:
	static unsigned int m_currentVAO;
	static ShaderProgram* m_currentShader;
private:
};

