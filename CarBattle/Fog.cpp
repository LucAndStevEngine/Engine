#include "Fog.h"
#include "Shader.h"


Fog::Fog() : density(0.15f), start(5.0f), end(75.0f), color(0.7f, 0.7f, 0.7f, 1.0f), equation(FOG_EQUATION_EXP2), enabled(false)
{
}


void Fog::SendToShader(ShaderProgram &program)
{
	program.SetUniform("fog.enabled", enabled);
	if (enabled)
	{
		program.SetUniform("fog.density", density);
		program.SetUniform("fog.start", start);
		program.SetUniform("fog.end", end);
		program.SetUniform("fog.fogColor", color);
		program.SetUniform("fog.equation", equation);
	}
}
