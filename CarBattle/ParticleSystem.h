#pragma once

#include "RenderComponent.h"
#include "GLMath.h"

#define NUM_PARTICLE_ATTRIBUTES 6
#define MAX_PARTICLES_ON_SCENE 100000

#define PARTICLE_TYPE_GENERATOR 0
#define PARTICLE_TYPE_NORMAL 1

class Particle
{
public:
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 colour;
	float lifeTime;
	float size;
	int type;
};

class ParticleSystem : public RenderComponent
{
public:
	//bool InitializeParticleSystem();

	virtual void Init() override;
	virtual void Render() override;
	virtual void Update() override;

	void ClearParticles();
	bool ReleaseParticleSystem();

	int GetNumOfParticles();

	void SetGenProperties(glm::vec3 minVelo, glm::vec3 maxVelo, glm::vec3 gravity, glm::vec3 color, float minLife, float maxLife, float size);
	void SetTexture(class Texture* texture);

	ParticleSystem();
	~ParticleSystem();

private:
	bool m_bIsInitialized;

	unsigned int m_transfomFeedbackBuffer;
	unsigned int m_particleBuffer[2];
	unsigned int m_VAO_ID_TWO;

	unsigned int* m_VAOs[2];

	unsigned int m_query;
	class Texture* m_texture;

	int m_currentReadBuffer;
	int m_numOfParticles;

	glm::vec3 quad1, quad2;

	float m_elapsedTime;
	float m_nextGenTime;

	glm::vec3 m_genVelocityMin, m_genVelocityMax;
	glm::vec3 m_genGravityVector;
	glm::vec3 m_genColor;

	float m_lifeMin, m_lifeMax;
	float m_size;

	int m_numToGenerate;
	
	ShaderProgram* m_shaderTwo;

	virtual unsigned int GetVAO() override;
	virtual ShaderProgram* GetShader() override;
};

