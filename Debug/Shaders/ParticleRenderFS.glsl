#version 400 core

uniform sampler2D texture;

smooth in vec2 texCoord;
flat in vec4 colorPart;

out vec4 FragColor;

void main()
{
	vec4 texColor = texture2D(texture, texCoord);
	FragColor = vec4(texColor.xyz, 1.0f) * colorPart;
}