#pragma once

#include "Shader.h"
#include <list>

class RenderComponent;

class RenderingManager
{
public:
	RenderingManager();

	void AddRenderComp(RenderComponent* renderer);
	void RemoveRenderComp(RenderComponent* renderer);

	void RenderScene();

protected:
	static unsigned int m_currentVAO;
	static ShaderProgram* m_currentShader;
private:
	std::list<RenderComponent*> renderers;	
};

