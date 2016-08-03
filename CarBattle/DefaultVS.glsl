#version 400 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec4 color;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 TexCoord;
out vec3 Normal;
out vec4 Color;
out vec4 EyePos;
out vec3 WorldPos;

void main()
{
	vec4 eyeSpacePos = view * model * vec4(position, 1.0f);
	gl_Position = projection * eyeSpacePos;//view * model * vec4(position, 1.0f);
	TexCoord = texCoord;

	vec4 vRes = transpose(inverse(model)) * vec4(normal, 0.0f);
	Normal = vRes.xyz;

	EyePos = eyeSpacePos;
	Color = color;

}