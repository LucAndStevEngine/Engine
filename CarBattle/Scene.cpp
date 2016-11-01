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
#include "ModelRenderer.h"
#include "Light.h"

//Car* car;

DirectionalLight* light;
SceneNode* node;

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
	
	ResourceManager::GetShader("Shaders/SkyboxFS.glsl", NULL, "Shaders/SkyboxVS.glsl", "Skybox");
	ResourceManager::GetShader("Shaders/DefaultFS.glsl", NULL, "Shaders/DefaultVS.glsl", "Default");

	light = new DirectionalLight();


	AssimpModel* model = new AssimpModel();
	model->LoadModelFromFile("models/BMW_M3_GTR/BMW_M3_GTR.obj");
	model->FinalizeVBO();

	ResourceManager::SaveModel(model, "models/BMW_M3_GTR/BMW_M3_GTR.obj");

	Skybox* skybox = new Skybox();
	sceneGraph->SetRoot(skybox);
	skybox->LoadSkybox("skyboxes/", "jajlands1_ft.jpg", "jajlands1_bk.jpg", "jajlands1_lf.jpg", "jajlands1_rt.jpg", "jajlands1_up.jpg", "jajlands1_dn.jpg");

	camera = new Camera();
	camera->transform.position.z = -10;
	camera->cameraWidth = game->GetWindowWidth();
	camera->cameraHeight = game->GetWindowHeight();
	sceneGraph->root->AddChild(camera);

	//car = new Car();
	//car->transform.position.x = 0;
	//car->transform.position.y = 0;
	//car->transform.position.z = 20;
	//car->transform.scale = glm::vec3(0.01f);

	//sceneGraph->root->AddChild(car);
	//car->Init();

	node = new SceneNode();
	node->transform.scale = glm::vec3(0.001f);
	node->transform.position.z = 5;
	sceneGraph->root->AddChild(node);
	
	ModelRenderer* firstRenderer;
	firstRenderer = new ModelRenderer(ResourceManager::GetModel("models/BMW_M3_GTR/BMW_M3_GTR.obj"), ResourceManager::GetShader("Default"));
	firstRenderer->bAlwaysDraw = true;
	node->AddComponent(firstRenderer);

	m_isInitialized = true;
}

void Scene::Render()
{	
	glm::mat4 proj = glm::perspective(45.0f, (float)game->GetWindowWidth() / (float)game->GetWindowHeight(), 0.01f, 1000.0f);
	camera->SetProjection(proj);

	ShaderProgram* program = ResourceManager::GetShader("Default");
	program->UseProgram();
	program->SetUniform("diffuse", 0);
	program->SetUniform("specular", 2);
	program->SetUniform("projection", camera->GetProjection());
	program->SetUniform("view", camera->LookAt());
	program->SetUniform("viewPos", camera->transform.position);
	program->SetUniform("fog.enabled", false);
	light->SendToShader(*program);

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
		camera->transform.euler.y -= 20.0f * Time::deltaTime;
	}

	if (InputManager::Instance().KeyPress(GLFW_KEY_A))
	{
		GetGame()->GetRenderManager()->PrintText("Left", glm::vec4(1, 1, 1, 1), glm::vec2(500, 30), 24, camera, "Dir");
	}
	if (InputManager::Instance().KeyDown(GLFW_KEY_A))
	{
		camera->transform.euler.y += 20.0f * Time::deltaTime;
	}

	if (InputManager::Instance().KeyPress(GLFW_KEY_W))
	{
		node->transform.position = camera->transform.position - camera->transform.forward;
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