#version 400 core

in vec2 TexCoord;
in vec4 EyeSpace;

out vec4 outputColor;

uniform sampler2D texture;
uniform vec4 color;

struct FogParameters
{
	vec4 fogColor;
	float start;
	float end;
	float density;

	int equation; // 0 = linear, 1 = exp, 2 = exp2
	bool enabled;
};

uniform FogParameters fog;

float GetFogFactor(FogParameters params, float fogCoord); 

void main()
{
	vec4 textureColor = texture2D(texture, TexCoord);
	outputColor = textureColor * color;

	
	if(fog.enabled)
	{
		float fogCoord = abs(EyeSpace.z / EyeSpace.w);
		outputColor = mix(outputColor, fog.fogColor, GetFogFactor(fog, fogCoord));
	}

}

float GetFogFactor(FogParameters params, float fogCoord)
{
	float result = 0.0f;

	if(params.equation == 0)
	{
      result = (params.end - fogCoord)/(params.end-params.start); 
	}
	else if(params.equation == 1)
	{ 
      result = exp(-params.density * fogCoord); 
	}
	else if(params.equation == 2)
	{
      result = exp(-pow(params.density * fogCoord, 2)); 
	}

	result = 1.0 - clamp(result, 0.0, 1.0); 

	return result;
}