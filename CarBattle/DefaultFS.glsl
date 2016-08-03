#version 400 core

in vec2 TexCoord;
in vec3 Normal;
in vec4 EyeSpace;
in vec3 WorldPos;
in vec4 Color;

out vec4 outputColor;

uniform sampler2D diffuse;
uniform sampler2D specular;

struct DirectionalLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;   
};

struct FogParameters
{
	vec4 fogColor;
	float start;
	float end;
	float density;

	int equation; // 0 = linear, 1 = exp, 2 = exp2
	bool enabled;
};

uniform DirectionalLight directionalLight;

#define NR_POINT_LIGHTS 50
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform int numPointLights;

#define NR_SPOT_LIGHTS 50
uniform SpotLight spotLights[NR_SPOT_LIGHTS];
uniform int numSpotLights;

uniform FogParameters fog;
uniform vec3 viewPos;

vec4 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec4 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
float GetFogFactor(FogParameters params, float fogCoord);

void main()
{
	vec3 viewDir = normalize(viewPos.xyz - WorldPos.xyz);

	vec4 result = CalcDirLight(directionalLight, Normal, viewDir);

	for(int i = 0; i < numPointLights; i++)
	{
		result += CalcPointLight(pointLights[i], Normal, WorldPos, viewDir);
	}

	for(int i = 0; i < numSpotLights; i++)
	{
		result += CalcSpotLight(spotLights[i], Normal, WorldPos, viewDir);
	}
	
	outputColor = result * Color;

	if(fog.enabled)
	{
		float fogCoord = abs(EyeSpace.z / EyeSpace.w);
		outputColor = mix(outputColor, fog.fogColor, GetFogFactor(fog, fogCoord));
	}
}

vec4 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 0.1f);
    // Combine results
    vec4 ambient = vec4(light.ambient, 1) * texture2D(diffuse, TexCoord);
    vec4 diffuse =  vec4(light.diffuse, 1) * diff * texture2D(diffuse, TexCoord);
    vec4 specular =  vec4(light.specular, 1) * spec * texture2D(specular, TexCoord);

    return (ambient + diffuse + specular);
}	


vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	// Diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);

	//specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 0.1f);

	// Attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// Combine results
    vec4 ambient = vec4(light.ambient, 1) * texture2D(diffuse, TexCoord);
    vec4 diffuse =  vec4(light.diffuse, 1) * diff * texture2D(diffuse, TexCoord);
    vec4 specular =  vec4(light.specular, 1) * spec * texture2D(specular, TexCoord);
	
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	
	return (ambient + diffuse + specular);
}

vec4 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 0.1f);
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // Spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // Combine results
    vec4 ambient = vec4(light.ambient, 1) * texture2D(diffuse, TexCoord);
    vec4 diffuse =  vec4(light.diffuse, 1) * diff * texture2D(diffuse, TexCoord);
    vec4 specular =  vec4(light.specular, 1) * spec * texture2D(specular, TexCoord);
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
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