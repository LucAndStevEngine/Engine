#include "Game.h"
#include "InputManager.h"
#include "WindowControl.h"
#include "Time.h"

#include "Skybox.h"

#include "Light.h"
#include "Camera.h"
#include "AssimpModel.h"
#include "ModelRenderer.h"
#include "ResourceManager.h"

AssimpModel model;
SceneNode* first;
SceneNode* two;
SceneNode* three;
SceneNode* four;

DirectionalLight dLight;
PointLight pLight;

btRigidBody* carBody;
btTransform carTrans;
btRigidBody* body;
Game::Game()
{

}

void Game::Init(WindowControl* windowControl)
{
	this->windowControl = windowControl;

	renderingManager = new RenderingManager();

	physicsManager = new PhysicsManager();
	physicsManager->SetGravity(&btVector3(0, -20, 0));
	physicsManager->Init();

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
	ResourceManager::SaveShader(program, "Skybox");

	program = new ShaderProgram();
	program->CreateProgram();
	program->AddShaderToProgram(shaders[2]);
	program->AddShaderToProgram(shaders[3]);
	program->LinkProgram();
	programs.push_back(program);
	ResourceManager::SaveShader(program, "Default");

	sceneGraph = new SceneGraph(this);

	skybox = new Skybox();
	sceneGraph->root->AddChild(skybox);
	skybox->LoadSkybox("skyboxes/", "jajlands1_ft.jpg", "jajlands1_bk.jpg", "jajlands1_lf.jpg", "jajlands1_rt.jpg", "jajlands1_up.jpg", "jajlands1_dn.jpg");

	camera = new Camera();
	camera->transform.position.z = -1;
	sceneGraph->root->AddChild(camera);
	
	cameraTwo = new Camera();
	cameraTwo->transform.position.z = 5;
	sceneGraph->root->AddChild(cameraTwo);

	model.LoadModelFromFile("models/BMW_M3_GTR/BMW_M3_GTR.obj");
	model.FinalizeVBO();

	first = new SceneNode();
	first->transform.scale = glm::vec3(0.001f);
	first->transform.position.z = 0;
	sceneGraph->root->AddChild(first);
	
	pLight.diffuse = glm::vec3(1, 0, 0);

	ModelRenderer* firstRenderer;
	firstRenderer = new ModelRenderer(&model, programs[1]);

	first->AddComponent(firstRenderer);

	//first = new SceneNode();
	//first->transform.scale = glm::vec3(0.001f);
	//first->transform.position.z = 0;
	//sceneGraph->root->AddChild(first);

	//pLight.diffuse = glm::vec3(1, 0, 0);

	//first->AddComponent(modelRender);
	//first->transform.position.z = -100;

	//first = new SceneNode();
	//first->transform.scale = glm::vec3(0.001f);
	//first->transform.position.z = 0;
	//sceneGraph->root->AddChild(first);

	//pLight.diffuse = glm::vec3(1, 0, 0);

	//first->AddComponent(modelRender);

	//first->transform.position.x = -400;
	
	btCollisionShape* carBox = new btBoxShape(btVector3(20, 5, 10));
	carTrans.setIdentity();
	glm::vec3 pos = first->transform.position;
	carTrans.setOrigin(btVector3(pos.x, pos.y, pos.z));
	btScalar mass(50.0f);
	btVector3 localInertia(1, 1, 1);
	carBox->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(carTrans);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, carBox, localInertia);
	body = new btRigidBody(rbInfo);

	physicsManager->AddCollisionShape(body);

}

void Game::Update()
{

	bool press = InputManager::Instance().GetJoyStickButtonDown(GLFW_JOYSTICK_1, 0);

	float axis = InputManager::Instance().GetJoyStickAxis(GLFW_JOYSTICK_1, 1);
	if (press)
	{
		camera->transform.position -= camera->transform.forward * Time::deltaTime * 20.0f;
	}

	sceneGraph->Update();

	glm::quat rot = first->transform.rotation;
	carTrans.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));
}	

void Game::FixedUpdate()
{
	sceneGraph->FixedUpdate();
	physicsManager->Update();

	btTransform trans;
	if (body && body->getMotionState())
	{
		body->getMotionState()->getWorldTransform(trans);
	}
	
	first->transform.position.x = trans.getOrigin().getX();
	first->transform.position.y = trans.getOrigin().getY();
	first->transform.position.z = trans.getOrigin().getZ();
}

void Game::Render()
{
	glViewport(0, 0, windowControl->GetWidth()/2 - 5, windowControl->GetHeight());
	ShaderProgram* program = ResourceManager::GetShader("Skybox");
	program->UseProgram();
	glm::mat4 proj = glm::perspective(45.0f, (((float)GetWindowWidth()/2)-5) / (float)GetWindowHeight(), 0.01f, 1000.0f);
	program->SetUniform("projection", proj);
	program->SetUniform("view", glm::translate(camera->LookAt(), camera->transform.position));
	program->SetUniform("model", glm::mat4());
	program->SetUniform("color", glm::vec4(1, 1, 1, 1));

	program->SetUniform("fog.enabled", false);

	program = ResourceManager::GetShader("Default");
	program->UseProgram();
	program->SetUniform("diffuse", 0);
	program->SetUniform("specular", 2);
	program->SetUniform("projection", proj);
	program->SetUniform("view", camera->LookAt());
	program->SetUniform("viewPos", camera->transform.position);
	program->SetUniform("fog.enabled", false);
	dLight.SendToShader(*program);
	program->SetUniform("numPointLights", 1);
	pLight.SendToShader(*program);
	
	renderingManager->frustum.CreateFrustum(camera->transform, 0.01f, 1000.0f, (((float)GetWindowWidth() / 2)-5)/ (float)GetWindowHeight(), 45.0f);
	renderingManager->RenderScene();


	glViewport(windowControl->GetWidth() / 2 + 5, 0, windowControl->GetWidth() / 2 - 5, windowControl->GetHeight());
	program = ResourceManager::GetShader("Skybox");
	program->UseProgram();
	program->SetUniform("view", glm::translate(cameraTwo->LookAt(), cameraTwo->transform.position));

	program = ResourceManager::GetShader("Default");
	program->UseProgram();
	program->SetUniform("view", cameraTwo->LookAt());
	program->SetUniform("viewPos", cameraTwo->transform.position);
	renderingManager->frustum.CreateFrustum(cameraTwo->transform, 0.01f, 1000.0f, (((float)GetWindowWidth() / 2) - 5) / (float)GetWindowHeight(), 45.0f);
	renderingManager->RenderScene();
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

RenderingManager* Game::GetRenderManager()
{
	return renderingManager;
}

Game::~Game()
{

}
