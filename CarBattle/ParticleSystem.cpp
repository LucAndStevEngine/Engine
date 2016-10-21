#include "ParticleSystem.h"
#include "GL\glew.h"
#include "Time.h"
#include "SceneNode.h"
#include <time.h>
#include "RenderingManager.h"
#include "Game.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "Texture.h"

ParticleSystem::ParticleSystem()
{
	m_bIsInitialized = false;
	m_currentReadBuffer = 0;
}


ParticleSystem::~ParticleSystem()
{
}

unsigned int ParticleSystem::GetVAO()
{
	return *m_VAOs[m_currentReadBuffer]; 
}

ShaderProgram * ParticleSystem::GetShader()
{
	return m_shaderTwo;
}

void ParticleSystem::Init()
{
	if (m_bIsInitialized)
		return;

	RenderComponent::Init();

	srand(time(NULL));

	m_shader = ResourceManager::GetShader("ParticleUpdate");
	m_shaderTwo = ResourceManager::GetShader("ParticleRender");

	if (m_shader == NULL)
	{
		const char* sVaryings[NUM_PARTICLE_ATTRIBUTES] =
		{
			"positionOut",
			"velocityOut",
			"colorOut",
			"lifeTimeOut",
			"sizeOut",
			"typeOut",
		};

		// Used for updating system

		Shader updateVertShader, updateGeomShader;
		updateVertShader.LoadShader("ParticleUpdateVS.glsl", GL_VERTEX_SHADER);
		updateGeomShader.LoadShader("ParticleUpdateGS.glsl", GL_GEOMETRY_SHADER);

		m_shader = new ShaderProgram();
		m_shader->CreateProgram();
		m_shader->AddShaderToProgram(&updateVertShader);
		m_shader->AddShaderToProgram(&updateGeomShader);
		for (int i = 0; i < NUM_PARTICLE_ATTRIBUTES; i++)
		{
			glTransformFeedbackVaryings(m_shader->GetProgramID(), 6, sVaryings, GL_INTERLEAVED_ATTRIBS);
		}
		m_shader->LinkProgram();
		ResourceManager::SaveShader(m_shader, "ParticleUpdate");
	}
	if (m_shaderTwo == NULL)
	{
		// Used for rendering the system

		Shader renderVertShader, renderGeoShader, renderFragShader;

		renderVertShader.LoadShader("ParticlesRenderVS.glsl", GL_VERTEX_SHADER);
		renderGeoShader.LoadShader("ParticlesRenderGS.glsl", GL_GEOMETRY_SHADER);
		renderFragShader.LoadShader("ParticlesRenderFS.glsl", GL_FRAGMENT_SHADER);


		m_shaderTwo = new ShaderProgram();
		m_shaderTwo->CreateProgram();
		m_shaderTwo->AddShaderToProgram(&renderVertShader);
		m_shaderTwo->AddShaderToProgram(&renderGeoShader);
		m_shaderTwo->AddShaderToProgram(&renderFragShader);
		m_shaderTwo->LinkProgram();
		ResourceManager::SaveShader(m_shaderTwo, "ParticleRender");
	}

	glGenTransformFeedbacks(1, &m_transfomFeedbackBuffer);
	glGenQueries(1, &m_query);
	glGenBuffers(2, m_particleBuffer);

	m_VAOs[0] = &m_VAO_ID;
	m_VAOs[1] = &m_VAO_ID_TWO;
	glGenVertexArrays(2, *m_VAOs);

	Particle particleInit;
	particleInit.type = PARTICLE_TYPE_GENERATOR;


	for (int i = 0; i < 2; i++)
	{
		glBindVertexArray(*m_VAOs[i]);
		glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Particle)*MAX_PARTICLES_ON_SCENE, NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Particle), &particleInit);

		for (int j = 0; j < NUM_PARTICLE_ATTRIBUTES; j++)
		{
			glEnableVertexAttribArray(j);
		}

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)0); // Position
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)12); // Velocity
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)24); // Color
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)36); // Lifetime
		glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)40); // Size
		glVertexAttribPointer(5, 1, GL_INT, GL_FALSE, sizeof(Particle), (const GLvoid*)44); // Type
	}

	m_currentReadBuffer = 0;
	m_numOfParticles = 1;

	m_bIsInitialized = true;
}


void ParticleSystem::Render()
{
	if (!m_bIsInitialized)
		return;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDepthMask(0);

	glDisable(GL_RASTERIZER_DISCARD);
	glDisableVertexAttribArray(1);

	m_shaderTwo->SetUniform("projection", m_renderManager->GetCurrentCamera()->GetProjection());
	m_shaderTwo->SetUniform("view", m_renderManager->GetCurrentCamera()->LookAt());
	m_shaderTwo->SetUniform("quad1", &quad1);
	m_shaderTwo->SetUniform("quad2", &quad2);
	m_shaderTwo->SetUniform("texture", 0);

	m_texture->BindTexture();



	glDrawArrays(GL_POINTS, 0, m_numOfParticles);

	glDepthMask(1);
}


void ParticleSystem::Update()
{
	if (!m_bIsInitialized)
		return;

	std::cout << m_numOfParticles << std::endl;

	ShaderProgram* lastProgram = m_renderManager->GetCurrentShader();
	unsigned int lastVAO = m_renderManager->GetCurrentVAO();

	if (m_renderManager->GetCurrentCamera() != NULL)
	{
		Camera *camera = m_renderManager->GetCurrentCamera();
		glm::vec3 normView = glm::normalize(camera->transform.forward);
		quad1 = glm::cross(normView, camera->transform.up);
		quad1 = glm::normalize(quad1);
		quad2 = glm::cross(normView, quad1);
		quad2 = glm::normalize(quad2);
	}

	m_shader->UseProgram();

	m_shader->SetUniform("DeltaTime", Time::deltaTime);
	m_shader->SetUniform("GenPosition", sceneNode->transform.position);
	m_shader->SetUniform("GenVelocityMin", m_genVelocityMin);
	m_shader->SetUniform("GenVelocityMax", m_genVelocityMax);
	m_shader->SetUniform("GenColor", m_genColor);
	m_shader->SetUniform("GenGravityVector", m_genGravityVector);

	m_shader->SetUniform("GenLifeMin", m_lifeMin);
	m_shader->SetUniform("GenLifeMax", m_lifeMax);

	m_shader->SetUniform("GenSize", m_size);
	m_shader->SetUniform("NumToGenerate", 0);

	m_elapsedTime += Time::deltaTime;

	if (m_elapsedTime > m_nextGenTime)
	{
		m_shader->SetUniform("NumToGenerate", m_numToGenerate);
		m_elapsedTime -= m_nextGenTime;


		glm::vec3 randomSeed = glm::vec3(randFloat(-10.0f, 20.0f), randFloat(-10.0f, 20.0f), randFloat(-10.0f, 20.0f));
		m_shader->SetUniform("RandomSeed", &randomSeed);
	}

	glEnable(GL_RASTERIZER_DISCARD);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transfomFeedbackBuffer);

	glBindVertexArray(*m_VAOs[m_currentReadBuffer]);
	glEnableVertexAttribArray(1); // Re-enable velocity

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_particleBuffer[1 - m_currentReadBuffer]);

	glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, m_query);
	glBeginTransformFeedback(GL_POINTS);

	glDrawArrays(GL_POINTS, 0, m_numOfParticles);

	glEndTransformFeedback();

	glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
	glGetQueryObjectiv(m_query, GL_QUERY_RESULT, &m_numOfParticles);

	m_currentReadBuffer = 1 - m_currentReadBuffer;

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	glDisable(GL_RASTERIZER_DISCARD);

	if (lastProgram != NULL)
	{
		lastProgram->UseProgram();
		glBindVertexArray(lastVAO);
	}



}

void ParticleSystem::ClearParticles()
{
}

bool ParticleSystem::ReleaseParticleSystem()
{
	glGenBuffers(2, m_particleBuffer);
	glDeleteBuffers(2, *m_VAOs);
	glDeleteQueries(1, &m_query);
	glDeleteTransformFeedbacks(1, &m_transfomFeedbackBuffer);

	return true;
}

int ParticleSystem::GetNumOfParticles()
{
	return m_numOfParticles;
}

void ParticleSystem::SetGenProperties(glm::vec3 minVelo, glm::vec3 maxVelo, glm::vec3 gravity, glm::vec3 color, float minLife, float maxLife, float size)
{
	m_genVelocityMin = minVelo;
	m_genVelocityMax = maxVelo;
	m_genGravityVector = gravity;
	m_genColor = color;
	m_lifeMin = minLife;
	m_lifeMax = maxLife;
	m_size = size;
	m_nextGenTime = 0.5f;
	m_numToGenerate = 500;
}

void ParticleSystem::SetTexture(Texture* tex)
{
	m_texture = tex;
}
