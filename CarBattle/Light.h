#pragma once
#include <string>
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Light
{
public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	Light();

	virtual void SendToShader(ShaderProgram &shader, int lightNum = 0) = 0;
};

class DirectionalLight : public Light
{
public:
	glm::vec3 direction;
	DirectionalLight();

	virtual void SendToShader(ShaderProgram &shader, int lightNum = 0) override;
};

class SpotLight : public Light
{
public:
	glm::vec3 position;
	glm::vec3 direction;
	float cutOff;
	float outerCutOff;
	
	float constant;
	float linear;
	float quadratic;

	SpotLight();

	virtual void SendToShader(ShaderProgram &shader, int lightNum = 0) override;
};

class PointLight : public Light
{
public:
	glm::vec3 position;

	float constant;
	float linear;
	float quadratic;

	PointLight();

	virtual void SendToShader(ShaderProgram &shader, int lightNum = 0) override;
};
