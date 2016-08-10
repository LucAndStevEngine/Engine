#include "Game.h"
#include "InputManager.h"
#include "WindowControl.h"
#include "Time.h"

#include "Skybox.h"

#include "Light.h"
#include "Camera.h"
#include "AssimpModel.h"
#include "ModelRenderer.h"

AssimpModel model;
SceneNode* first;
ModelRenderer* modelRender;

DirectionalLight dLight;
PointLight pLight;

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

	Shader* shader = new Shader();
	shader->LoadShader("SkyboxFS.glsl", GL_FRAGMENT_SHADER);
	shaders.push_back(shader);
	shader = new Shader();
	shader->LoadShader("SkyboxVS.glsl", GL_VERTEX_SHADER);
	shaders.push_back(shader);

	shader = new Shader();
	shader->LoadShader("DefaultVS.glsl", GL_VERTEX_SHADER);
	shaders.push_back(shader);
	shader = new Shader();
	shader->LoadShader("DefaultFS.glsl", GL_FRAGMENT_SHADER);
	shaders.push_back(shader);

	ShaderProgram* program = new ShaderProgram();
	program->CreateProgram();
	program->AddShaderToProgram(shaders[0]);
	program->AddShaderToProgram(shaders[1]);
	program->LinkProgram();
	programs.push_back(program);

	program = new ShaderProgram();
	program->CreateProgram();
	program->AddShaderToProgram(shaders[2]);
	program->AddShaderToProgram(shaders[3]);
	program->LinkProgram();
	programs.push_back(program);

	skybox = new Skybox();
	skybox->LoadSkybox("skyboxes/", "jajlands1_ft.jpg", "jajlands1_bk.jpg", "jajlands1_lf.jpg", "jajlands1_rt.jpg", "jajlands1_up.jpg", "jajlands1_dn.jpg");

	sceneGraph = new SceneGraph(this);
	camera = new Camera();
	camera->transform.position.z = -1;
	sceneGraph->root->AddChild(camera);

	model.LoadModelFromFile("models/BMW_M3_GTR/BMW_M3_GTR.obj");
	model.FinalizeVBO();

	first = new SceneNode();
	first->transform.scale = glm::vec3(0.001f);
	first->transform.position.z = 0;
	sceneGraph->root->AddChild(first);
	
	pLight.diffuse = glm::vec3(1, 0, 0);

	modelRender = new ModelRenderer(&model, programs[1]);

	first->AddComponent(modelRender);

	renderingManager.AddRenderComp(modelRender);

	
}

void Game::Update()
{
	if (InputManager::Instance().KeyPress(GLFW_KEY_A))
	{
		camera->transform.euler.y += Time::deltaTime;
	}

	if (InputManager::Instance().KeyPress(GLFW_KEY_D))
	{
		camera->transform.euler.y -= Time::deltaTime;
	}

	if (InputManager::Instance().KeyPress(GLFW_KEY_W))
	{
		camera->transform.position += camera->transform.forward * Time::deltaTime;
	}

	if (InputManager::Instance().KeyPress(GLFW_KEY_S))
	{
		camera->transform.position -= camera->transform.forward * Time::deltaTime;
	}

	sceneGraph->Update();
	std::cout << "FPS -  " << Time::FPS << std::endl;
}	

void Game::FixedUpdate()
{
	sceneGraph->FixedUpdate();
	first->transform.euler.y += Time::fixedDeltaTime;
}

void Game::Render()
{
	//programs[0]->UseProgram();
	//programs[0]->SetUniform("projection", glm::perspective(45.0f, (float)(GetWindowWidth()/GetWindowHeight()), 0.01f, 10000.0f));
	//programs[0]->SetUniform("view", glm::translate(camera->LookAt(), camera->transform.position));
	//programs[0]->SetUniform("model", glm::mat4());
	//programs[0]->SetUniform("color", glm::vec4(1, 1, 1, 1));

	//programs[0]->SetUniform("fog.enabled", false);
	//skybox->Render();

	programs[1]->UseProgram();
	programs[1]->SetUniform("diffuse", 0);
	programs[1]->SetUniform("specular", 2);
	programs[1]->SetUniform("projection", glm::perspective(45.0f, (float)(GetWindowWidth() / GetWindowHeight()), 0.01f, 10000.0f));
	programs[1]->SetUniform("view", camera->LookAt());
	programs[1]->SetUniform("viewPos", camera->transform.position);
	programs[1]->SetUniform("fog.enabled", false);
	dLight.SendToShader(*programs[1]);
	programs[1]->SetUniform("numPointLights", 1);
	pLight.SendToShader(*programs[1]);
	
	renderingManager.RenderScene();
}

void Game::Shutdown()
{

	delete skybox;
	delete sceneGraph;
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
