#version 400 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 TexCoord;
out vec4 EyeSpace;


void main()
{
	vec4 eyeSpacePos = view * vec4(position, 1.0f);
	gl_Position = projection * view * vec4(position, 1.0f);
	TexCoord = texCoord;

	EyeSpace = eyeSpacePos;
}
