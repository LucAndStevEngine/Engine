#version 400 core

in vec2 texCoord;
out vec4 outputColor;

uniform sampler2D gSampler;
uniform vec4 vColor;

void main()
{
	vec4 vTexColor = texture2D(gSampler, texCoord);
	outputColor = vec4(vTexColor.r, vTexColor.r, vTexColor.r, vTexColor.r)*vColor;
}