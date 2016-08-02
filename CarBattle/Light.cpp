#include "Light.h"



Light::Light() : ambient(0.0f, 0.0f, 0.0f), diffuse(1, 1, 1), specular(1, 1, 1)
{
}

DirectionalLight::DirectionalLight() : direction(0, 0, 1)
{

}

PointLight::PointLight() : position(0, 0, 0), constant(1.0f), linear(0.009), quadratic(0.0032)
{
}

SpotLight::SpotLight() : position(0, 0, 0), direction(0, 0, 1), cutOff(0.91), outerCutOff(0.82), constant(1.0f), linear(0.09f), quadratic(0.032f)
{

}






void DirectionalLight::SendToShader(ShaderProgram &program, int lightNum)
{
	program.SetUniform("directionalLight.direction", direction);
	program.SetUniform("directionalLight.ambient", ambient);
	program.SetUniform("directionalLight.diffuse", diffuse);
	program.SetUniform("directionalLight.specular", specular);
}

void PointLight::SendToShader(ShaderProgram &program, int lightNum)
{
	std::string loc = "pointLights[" + std::to_string(lightNum) + "].";

	program.SetUniform((loc + "position").c_str(), position);
	program.SetUniform((loc + "ambient").c_str(), ambient);
	program.SetUniform((loc + "diffuse").c_str(), diffuse);
	program.SetUniform((loc + "specular").c_str(), specular);
	program.SetUniform((loc + "constant").c_str(), constant);
	program.SetUniform((loc + "linear").c_str(), linear);
	program.SetUniform((loc + "quadratic").c_str(), quadratic);
}

void SpotLight::SendToShader(ShaderProgram &program, int lightNum)
{
	std::string loc = "spotLights[" + std::to_string(lightNum) + "].";
	program.SetUniform(loc + "position", position);
	program.SetUniform(loc + "ambient", ambient);
	program.SetUniform(loc + "diffuse", diffuse);
	program.SetUniform(loc + "specular", specular);
	program.SetUniform(loc + "constant", constant);
	program.SetUniform(loc + "linear", linear);
	program.SetUniform(loc + "quadratic", quadratic);
	program.SetUniform(loc + "position", position);
	program.SetUniform(loc + "direction", direction);
	program.SetUniform(loc + "cutOff", cutOff);
	program.SetUniform(loc + "outerCutOff", outerCutOff);
}