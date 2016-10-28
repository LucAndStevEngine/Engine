#include "Scene.h"
#include "InputManager.h"
#include "Time.h"
#include <string>

#include "GLMath.h"
#include "Game.h"
#include "SceneGraph.h"
#include "RenderingManager.h"
#include "PhysicsManager.h"
#include "SoundManager.h"
#include "Shader.h"
#include "ResourceManager.h"
#include "Skybox.h"
#include "GLFW\glfw3.h"
#include "Camera.h"
#include "Car.h"
Car* car;
Scene::Scene(Game* newGame) : m_isInitialized(false), game(newGame)
{
}


Scene::~Scene()
{
	delete soundManager;
	delete physicsManager;
	delete renderingManager;
	delete sceneGraph;
}

void Scene::Init()
{
	if (m_isInitialized)
	{
		return;
	}

	
	renderingManager = new RenderingManager();
	renderingManager->InitText("arial.ttf", 32, 1028);

	physicsManager = new PhysicsManager();
	physicsManager->SetGravity(&btVector3(0, -20, 0));
	physicsManager->Init();

	soundManager = new SoundManager();
	soundManager->Init();

	sceneGraph = new SceneGraph(game);

	Shader* shader = new Shader();
	shader->LoadShader("Shaders/SkyboxFS.glsl", GL_FRAGMENT_SHADER);
	ResourceManager::SaveIndividualShader(shader, "SkyboxFS");
	shader = new Shader();
	shader->LoadShader("Shaders/SkyboxVS.glsl", GL_VERTEX_SHADER);
	ResourceManager::SaveIndividualShader(shader, "SkyboxVS");

	ShaderProgram* program = new ShaderProgram();
	program->CreateProgram();
	program->AddShaderToProgram(ResourceManager::GetIndividualShader("SkyboxFS"));
	program->AddShaderToProgram(ResourceManager::GetIndividualShader("SkyboxVS"));
	program->LinkProgram();
	
	ResourceManager::SaveShader(program, "Skybox");

	Skybox* skybox = new Skybox();
	sceneGraph->SetRoot(skybox);
	skybox->LoadSkybox("skyboxes/", "jajlands1_ft.jpg", "jajlands1_bk.jpg", "jajlands1_lf.jpg", "jajlands1_rt.jpg", "jajlands1_up.jpg", "jajlands1_dn.jpg");

	camera = new Camera();
	camera->transform.position.z = 0;
	camera->cameraWidth = game->GetWindowWidth();
	camera->cameraHeight = game->GetWindowHeight();
	sceneGraph->root->AddChild(camera);


	Shader* Steve = new Shader();
	Steve->LoadShader("Shaders/DefaultFS.glsl", GL_FRAGMENT_SHADER);
	ResourceManager::SaveIndividualShader(shader, "DefaultFS");
	Steve = new Shader();
	Steve->LoadShader("Shaders/DefaultVS.glsl", GL_VERTEX_SHADER);
	ResourceManager::SaveIndividualShader(shader, "DefaultVS");
	ShaderProgram* carShader = new ShaderProgram();
	carShader->CreateProgram();
	carShader->AddShaderToProgram(ResourceManager::GetIndividualShader("DefaultFS"));
	carShader->AddShaderToProgram(ResourceManager::GetIndividualShader("DefaultVS"));
	carShader->LinkProgram();
	ResourceManager::SaveShader(carShader, "Default");
	 car = new Car();
	car->transform.position.x = 0;
	car->transform.position.y = 0;
	car->transform.position.z = -3;
	car->transform.scale.x = 1000;


	sceneGraph->root->AddChild(car);
	car->Init();


	m_isInitialized = true;
}

void Scene::Render()
{	
	glm::mat4 proj = glm::perspective(45.0f, (float)game->GetWindowWidth() / (float)game->GetWindowHeight(), 0.01f, 1000.0f);
	camera->SetProjection(proj);

	renderingManager->frustum.CreateFrustum(camera->transform, 0.01f, 1000.0f, (float)game->GetWindowWidth() / (float)game->GetWindowHeight(), 45.0f);

	renderingManager->RenderScene(camera);
}

void Scene::Update()
{
	bool press = InputManager::Instance().MousePress(GLFW_MOUSE_BUTTON_1);

	if (InputManager::Instance().KeyPress(GLFW_KEY_D))
	{
		GetGame()->GetRenderManager()->PrintText("Right" , glm::vec4(1, 1, 1, 1), glm::vec2(500, 30), 24, camera, "Dir");
	}
	if (InputManager::Instance().KeyDown(GLFW_KEY_D))
	{
		camera->transform.euler.y -= 10.0f * Time::deltaTime;
	}

	if (InputManager::Instance().KeyPress(GLFW_KEY_A))
	{
		GetGame()->GetRenderManager()->PrintText("Left", glm::vec4(1, 1, 1, 1), glm::vec2(500, 30), 24, camera, "Dir");
	}
	if (InputManager::Instance().KeyDown(GLFW_KEY_A))
	{
		camera->transform.euler.y += 10.0f * Time::deltaTime;
	}

	GetGame()->GetRenderManager()->PrintText("FPS - " + std::to_string(Time::FPS), glm::vec4(1, 1, 1, 1), glm::vec2(30, 30), 24, camera, "FPS");

	sceneGraph->Update();
	soundManager->Update();
}

void Scene::FixedUpdate()
{
	sceneGraph->FixedUpdate();
	physicsManager->Update();
}

void Scene::Shutdown()
{
	delete this;
}

RenderingManager * Scene::GetRenderManager()
{
	return renderingManager;
}

PhysicsManager * Scene::GetPhysicsManager()
{
	return physicsManager;
}

SoundManager * Scene::GetSoundManager()
{
	return soundManager;
}

Game * Scene::GetGame()
{
	return game;
}