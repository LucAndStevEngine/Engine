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

	void RenderScene(class Camera* cam);

	std::list<RenderComponent*> renderers;

	FrustumCulling frustum;

	ShaderProgram* GetCurrentShader();
	unsigned int GetCurrentVAO();
	class Camera* GetCurrentCamera();

protected:
	static class Camera* m_currentCamera;;
	static ShaderProgram* m_currentShader;
	static unsigned int m_currentVAO;
private:
};

