#pragma once
#include "Component.h"
#include "Shader.h"

class RenderComponent :	public Component
{
public:
	RenderComponent();
	RenderComponent(ShaderProgram* shader);
	RenderComponent(ShaderProgram* shader, unsigned int VAO);

	void SetShader(ShaderProgram* shader);
	virtual ShaderProgram* GetShader();

	void SetVAO(unsigned int VAO);
	virtual unsigned int GetVAO();

	virtual void Render();
	virtual ~RenderComponent();
	
	void Init() override;

	float radius = 0;
	bool bAlwaysDraw = false;

protected:
	ShaderProgram* m_shader;
	unsigned int m_VAO_ID;

	class RenderingManager* m_renderManager;
};

