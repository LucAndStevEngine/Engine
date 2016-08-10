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
	ShaderProgram* GetShader();

	void SetVAO(unsigned int VAO);
	unsigned int GetVAO();

	virtual void Render();
	virtual ~RenderComponent();
	
protected:
	ShaderProgram* m_shader;
	unsigned int m_VAO_ID;
};

