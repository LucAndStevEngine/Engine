#pragma once

#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

enum fogEquation
{
	FOG_EQUATION_LINEAR = 0,
	FOG_EQUATION_EXP,
	FOG_EQUATION_EXP2
};

class Fog
{
public:
	float density;
	float start;
	float end;
	glm::vec4 color;
	int equation;
	bool enabled;

	Fog();
	void SendToShader(class ShaderProgram &program);
};

