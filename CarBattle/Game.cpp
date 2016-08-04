#include "Game.h"

#include "WindowControl.h"

#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "Time.h"
#include "InputManager.h"

#include "Skybox.h"
#include "AssimpModel.h"
#include "Light.h"
#include "FrameBuffer.h"

#define FOG_EQUATION_LINEAR 0 
#define FOG_EQUATION_EXP 1 
#define FOG_EQUATION_EXP2 2 

namespace FogParameters
{
	float density = 0.15f;
	float start = 5.0f;
	float end = 75.0f;
	glm::vec4 color = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
	int equation = FOG_EQUATION_EXP2;
	bool enabled = false;
}

AssimpModel amModels[3];
DirectionalLight dLight;
PointLight pLight;
SpotLight sLight;
FrameBuffer frameBuffer;

Game::Game()
{

}

void Game::Init(WindowControl* windowControl)
{
	this->windowControl = windowControl;

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	fShader[0].LoadShader("DefaultFS.glsl", GL_FRAGMENT_SHADER);
	vShader[0].LoadShader("DefaultVS.glsl", GL_VERTEX_SHADER);

	shaderProgram[0] = new ShaderProgram();
	shaderProgram[0]->CreateProgram();
	shaderProgram[0]->AddShaderToProgram(&fShader[0]);
	shaderProgram[0]->AddShaderToProgram(&vShader[0]);
	shaderProgram[0]->LinkProgram();
	shaderProgram[0]->UseProgram();


	fShader[1].LoadShader("SkyboxFS.glsl", GL_FRAGMENT_SHADER);
	vShader[1].LoadShader("SkyboxVS.glsl", GL_VERTEX_SHADER);

	shaderProgram[1] = new ShaderProgram();
	shaderProgram[1]->CreateProgram();
	shaderProgram[1]->AddShaderToProgram(&fShader[1]);
	shaderProgram[1]->AddShaderToProgram(&vShader[1]);
	shaderProgram[1]->LinkProgram();
	shaderProgram[1]->UseProgram();

	float fVert[]
	{
		// Positions           // Normals           // Texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1,1,1,1,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f, 1,1,1,1,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f, 1,1,1,1,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f, 1,1,1,1,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1,1,1,1,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1,1,1,1,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f, 1,1,1,1,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f, 1,1,1,1,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f, 1,1,1,1,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f, 1,1,1,1,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f, 1,1,1,1,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f, 1,1,1,1,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1,1,1,1,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1,1,1,1,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 1,1,1,1,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 1,1,1,1,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f, 1,1,1,1,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1,1,1,1,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1,1,1,1,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1,1,1,1,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 1,1,1,1,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 1,1,1,1,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f, 1,1,1,1,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1,1,1,1,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1,1,1,1,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f, 1,1,1,1,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f, 1,1,1,1,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f, 1,1,1,1,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1,1,1,1,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1,1,1,1,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1,1,1,1,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f, 1,1,1,1,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, 1,1,1,1,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, 1,1,1,1,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1,1,1,1,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1,1,1,1,
	};

	vboGameObject.CreateVBO();

	// Generate VAO
	glGenVertexArrays(1, &VAO);
	// Setup of VAO
	glBindVertexArray(VAO);
	
	vboGameObject.BindVBO();
	for (int i = 0; i < sizeof(fVert); i++)
	{
		vboGameObject.AddData(&fVert[i], sizeof(float));
	}
	
	vboGameObject.UploadDataToGPU(GL_STATIC_DRAW);
	
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// texture attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// normal attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// normal attribute
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
		
	skybox.LoadSkybox("skyboxes/", "jajlands1_ft.jpg", "jajlands1_bk.jpg", "jajlands1_lf.jpg", "jajlands1_rt.jpg", "jajlands1_up.jpg", "jajlands1_dn.jpg");

	amModels[0].LoadModelFromFile("models/Deadpool/dead 123456.obj");
	//amModels[1].LoadModelFromFile("models/house/house.3ds");
	//amModels[2].LoadModelFromFile("models/Content/jeep.obj");

	AssimpModel::FinalizeVBO();
	pLight.position = glm::vec3(0, 1, 10);
	pLight.diffuse = glm::vec3(1, 0, 0);
	camera.position.y = 1;
	sLight.diffuse = glm::vec3(0, 1, 0);

	frameBuffer.CreateFrameBufferWithTexture(512, 256);
	frameBuffer.AddDepthBufer();
	frameBuffer.SetFrameBufferTextureFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_TRILINEAR);
}

void Game::Update()
{
	camera.Update();
	sLight.position = camera.position;
	sLight.direction = camera.forwardVector;

	if (InputManager::Instance().KeyPress(GLFW_KEY_D))
	{
		camera.rotation.y -= Time::deltaTime;
	}

	if (InputManager::Instance().KeyPress(GLFW_KEY_A))
	{
		camera.rotation.y += Time::deltaTime;
	}

	if (InputManager::Instance().KeyPress(GLFW_KEY_W))
	{
		camera.position += Time::deltaTime * 20 * camera.forwardVector;
	}

	if (InputManager::Instance().KeyPress(GLFW_KEY_S))
	{
		camera.position -= Time::deltaTime * 20 * camera.forwardVector;
	}

}	

void Game::FixedUpdate()
{

}

float rotation = 0;

void Game::Render()
{

	frameBuffer.BindFrameBuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shaderProgram[1]->UseProgram();
	camera.rotation.y += 180;
	camera.Update();
	glm::mat4 view = camera.LookAt();
	glm::mat4 proj = glm::perspective(45.0, (double)GetWindowWidth() / (double)GetWindowHeight(), 0.1, 10000.0);

	shaderProgram[1]->SetUniform("projection", proj);
	shaderProgram[1]->SetUniform("view", glm::translate(view, camera.position));
	shaderProgram[1]->SetUniform("color", glm::vec4(1, 1, 1, 1));

	shaderProgram[1]->SetUniform("fog.enabled", FogParameters::enabled);
	if (FogParameters::enabled)
	{
		shaderProgram[1]->SetUniform("fog.density", FogParameters::density);
		shaderProgram[1]->SetUniform("fog.start", FogParameters::start);
		shaderProgram[1]->SetUniform("fog.end", FogParameters::end);
		shaderProgram[1]->SetUniform("fog.fogColor", FogParameters::color);
		shaderProgram[1]->SetUniform("fog.equation", FogParameters::equation);
	}

	glm::mat4 model;

	skybox.Render();
	
	shaderProgram[0]->UseProgram();
	shaderProgram[0]->SetUniform("diffuse", 0);
	shaderProgram[0]->SetUniform("specular", 1);
	shaderProgram[0]->SetUniform("color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	dLight.SendToShader(*shaderProgram[0]);
	shaderProgram[0]->SetUniform("viewPos", camera.position);
	//shaderProgram[0]->SetUniform("numPointLights", 1);
	//pLight.SendToShader(*shaderProgram[0], 0);

	shaderProgram[0]->SetUniform("numSpotLights", 1);
	sLight.SendToShader(*shaderProgram[0], 0);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(100, -100, 0));
	rotation += Time::deltaTime;
	model = glm::rotate(model, rotation, glm::vec3(0, 1, 0));

	shaderProgram[0]->SetUniform("projection", proj);
	shaderProgram[0]->SetUniform("view", view);
	shaderProgram[0]->SetUniform("model", model);
	model = glm::mat4();

	shaderProgram[0]->SetUniform("fog.enabled", FogParameters::enabled);
	if(FogParameters::enabled)
	{
		shaderProgram[0]->SetUniform("fog.density", FogParameters::density);
		shaderProgram[0]->SetUniform("fog.start", FogParameters::start);
		shaderProgram[0]->SetUniform("fog.end", FogParameters::end);
		shaderProgram[0]->SetUniform("fog.fogColor", FogParameters::color);
		shaderProgram[0]->SetUniform("fog.equation", FogParameters::equation);
	}
	
	AssimpModel::BindModelsVAO();

	amModels[0].RenderModel();
	amModels[1].RenderModel();
	amModels[2].RenderModel();


	glBindFramebuffer(GL_FRAMEBUFFER, NULL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	windowControl->ResizeWindow(windowControl->GetWidth(), windowControl->GetHeight());

	shaderProgram[1]->UseProgram();
	camera.rotation.y -= 180;
	camera.Update();
	view = camera.LookAt();
	//glm::mat4 proj = glm::perspective(45.0, (double)GetWindowWidth() / (double)GetWindowHeight(), 0.1, 10000.0);

	shaderProgram[1]->SetUniform("projection", proj);
	shaderProgram[1]->SetUniform("view", glm::translate(view, camera.position));
	shaderProgram[1]->SetUniform("color", glm::vec4(1, 1, 1, 1));

	shaderProgram[1]->SetUniform("fog.enabled", FogParameters::enabled);
	if (FogParameters::enabled)
	{
		shaderProgram[1]->SetUniform("fog.density", FogParameters::density);
		shaderProgram[1]->SetUniform("fog.start", FogParameters::start);
		shaderProgram[1]->SetUniform("fog.end", FogParameters::end);
		shaderProgram[1]->SetUniform("fog.fogColor", FogParameters::color);
		shaderProgram[1]->SetUniform("fog.equation", FogParameters::equation);
	}

	model = glm::mat4();

	skybox.Render();

	shaderProgram[0]->UseProgram();
	shaderProgram[0]->SetUniform("diffuse", 0);
	shaderProgram[0]->SetUniform("specular", 1);
	shaderProgram[0]->SetUniform("color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	dLight.SendToShader(*shaderProgram[0]);
	shaderProgram[0]->SetUniform("viewPos", camera.position);
	shaderProgram[0]->SetUniform("numPointLights", 1);
	pLight.SendToShader(*shaderProgram[0], 0);

	shaderProgram[0]->SetUniform("numSpotLights", 1);
	sLight.SendToShader(*shaderProgram[0], 0);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(100, -100, 0));
	rotation += Time::deltaTime;
	model = glm::rotate(model, rotation, glm::vec3(0, 1, 0));

	shaderProgram[0]->SetUniform("projection", proj);
	shaderProgram[0]->SetUniform("view", view);
	shaderProgram[0]->SetUniform("model", model);
	model = glm::mat4();

	shaderProgram[0]->SetUniform("fog.enabled", FogParameters::enabled);
	if (FogParameters::enabled)
	{
		shaderProgram[0]->SetUniform("fog.density", FogParameters::density);
		shaderProgram[0]->SetUniform("fog.start", FogParameters::start);
		shaderProgram[0]->SetUniform("fog.end", FogParameters::end);
		shaderProgram[0]->SetUniform("fog.fogColor", FogParameters::color);
		shaderProgram[0]->SetUniform("fog.equation", FogParameters::equation);
	}

	AssimpModel::BindModelsVAO();

	amModels[0].RenderModel();
	amModels[1].RenderModel();
	amModels[2].RenderModel();


	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0, 0, 100));
	model = glm::scale(model, glm::vec3(100, 100, 1));
	shaderProgram[0]->SetUniform("model", model);
	
	model = glm::mat4();

	glBindVertexArray(VAO);
	frameBuffer.BindFrameBufferTexture(0, true);

	glDrawArrays(GL_TRIANGLES, 0, 36);

}


void Game::Shutdown()
{
	skybox.ReleaseSkybox();
	test.ReleaseTexture();
	glDeleteVertexArrays(1, &VAO);
	vboGameObject.ReleaseVBO();
}

int Game::GetWindowHeight()
{
	return windowControl->GetHeight();
}

int Game::GetWindowWidth()
{
	return windowControl->GetWidth();
}

Game::~Game()
{

}
