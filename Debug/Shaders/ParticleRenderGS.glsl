#version 400

uniform mat4 projection;
uniform mat4 view;

uniform vec3 quad1, quad2;

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

in vec3 colorPass[];
in float lifeTimePass[];
in float sizePass[];
in float typePass[];

smooth out vec2 texCoord;
flat out vec4 colorPart;

void main()
{
	if(typePass[0] != 0)
	{
		vec3 posOld = gl_Position.xyz;
		float size = sizePass[0];
		mat4 mVP = projection * view;

		colorPart = vec4(colorPass[0], lifeTimePass[0]);

		vec3 pos = posOld + (-quad1 - quad2) * size;
		texCoord = vec2(0.0, 0.0);
		gl_Position = mVP * vec4(pos, 1.0);
		EmitVertex();

		pos = posOld + (-quad1 + quad2) * size;
		texCoord = vec2(0.0, 1.0);
		gl_Position = mVP * vec4(pos, 1.0);
		EmitVertex();

		pos = posOld + (quad1 - quad2) * size;
		texCoord = vec2(1.0, 0.0);
		gl_Position = mVP * vec4(pos, 1.0);
		EmitVertex();

		pos = posOld + (quad1 + quad2) * size;
		texCoord = vec2(1.0, 1.0);
		gl_Position = mVP * vec4(pos, 1.0);
		EmitVertex();

		EndPrimitive();
	}
}

