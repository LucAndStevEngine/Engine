#version 400 core

layout (points) in;
layout (points) out;

layout (max_vertices = 40) out;

// All the data we get from vertex shader

in vec3 positionPass[];
in vec3 velocityPass[];
in vec3 colorPass[];
in float lifeTimePass[];
in float sizePass[];
in int typePass[];

// Info that we send even farther

out vec3 positionOut;
out vec3 velocityOut;
out vec3 colorOut;
out float lifeTimeOut;
out float sizeOut;
out int typeOut;

uniform vec3 GenPosition;
uniform vec3 GenGravityVector;
uniform vec3 GenVelocityMin, GenVelocityMax;

uniform vec3 GenColor;
uniform float GenSize;

uniform float GenLifeMin, GenLifeMax;
uniform float DeltaTime;

uniform vec3 RandomSeed;
vec3 localSeed;

uniform int NumToGenerate;

float RandZeroOne()
{
	uint n = floatBitsToUint(localSeed.y * 214013.0 + localSeed.x * 2531011.0 + localSeed.z * 141251.0);
	n = n * (n * n * 15711u + 789221u);
	n = (n >> 9u) | 0x3F800000u;

	float res =  2.0 - uintBitsToFloat(n);
    localSeed = vec3(localSeed.x + 147158.0 * res, localSeed.y * res  + 415161.0 * res, localSeed.z + 324154.0 * res);
    return res;
}

void main()
{
	localSeed = RandomSeed;

	// Rendering doesnt get called, so discard gl_position

	positionOut = positionPass[0];
	velocityOut = velocityPass[0];

	if(typePass[0] != 0)
	positionOut += velocityOut * DeltaTime;
	if(typePass[0] != 0)
	velocityOut += GenGravityVector * DeltaTime;

	colorOut = colorPass[0];
	lifeTimeOut = lifeTimePass[0] - DeltaTime;
	sizeOut = sizePass[0];
	typeOut = typePass[0];

	if(typeOut == 0)
	{
		EmitVertex();
		EndPrimitive();
    
		for(int i = 0; i < NumToGenerate; i++)
		{
			  positionOut = GenPosition;
			  velocityOut = GenVelocityMin + vec3(GenVelocityMax.x * RandZeroOne(), GenVelocityMax.y * RandZeroOne(), GenVelocityMax.z * RandZeroOne());
			  colorOut = GenColor;
			  lifeTimeOut = GenLifeMin + GenLifeMax * RandZeroOne();
			  sizeOut = GenSize;
			  typeOut = 1;

			  EmitVertex();
			  EndPrimitive();
	    }
  }
  else if(lifeTimeOut > 0.0)
  {
      EmitVertex();
      EndPrimitive(); 
  }
}