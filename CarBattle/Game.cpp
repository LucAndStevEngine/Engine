#include "Game.h"
#include "InputManager.h"
#include "WindowControl.h"
#include "Time.h"


#include "Scene.h"

Game::Game()
{

}

void Game::Init(WindowControl* windowControl)
{
	this->windowControl = windowControl;
	
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GetCurrentScene()->Init();
}

void Game::Update()
{
	GetCurrentScene()->Update();
}	

void Game::FixedUpdate()
{
	GetCurrentScene()->FixedUpdate();
}

void Game::Render()
{
	GetCurrentScene()->Render();
}

void Game::Shutdown()
{
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
	return GetCurrentScene()->GetRenderManager();
}

PhysicsManager * Game::GetPhysicsManager()
{
	return GetCurrentScene()->GetPhysicsManager();
}

SoundManager * Game::GetSoundManager()
{
	return GetCurrentScene()->GetSoundManager();
}

SceneGraph * Game::GetSceneGraph()
{
	return GetCurrentScene()->sceneGraph;
}

Scene* Game::GetCurrentScene()
{
	if (m_sceneStack.empty())
	{
		PushScene(new Scene(this));
	}
	return m_sceneStack.top();
}

void Game::PushScene(Scene * newScene)
{
	m_sceneStack.push(newScene);

	newScene->Init();
}

void Game::PopScene()
{
	Scene* scene = m_sceneStack.top();
	scene->Shutdown();

	m_sceneStack.pop();
	if (m_sceneStack.empty())
	{
		PushScene(new Scene(this));
	}
}

void Game::ClearScenes()
{
	while(!m_sceneStack.empty())
	{
		m_sceneStack.pop();
	}

	PushScene(new Scene(this));
}

Game::~Game()
{

}
