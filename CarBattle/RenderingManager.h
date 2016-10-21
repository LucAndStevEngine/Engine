#pragma once

#include "Shader.h"
#include <list>
#include "RenderComponent.h"
#include "FrustumCulling.h"
#include "TextRenderer.h"

struct TextData
{
	std::string text;
	glm::vec4 color;
	glm::vec2 position;
	float fontSize;

	class SceneNode* owner;
	std::string id;
};

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

	void InitText(std::string fontLocation, int pixelSize, int maxCharSupport = 128);
	void PrintText(std::string text, glm::vec4 color, glm::vec2 position, float fontSize, class SceneNode* owner, std::string id);
	void RemoveText(class SceneNode* owner, std::string id);

protected:
	static class Camera* m_currentCamera;;
	static ShaderProgram* m_currentShader;
	static unsigned int m_currentVAO;

private:
	std::vector<TextData*> m_textData;
	TextRenderer* m_textRenderer;
};

