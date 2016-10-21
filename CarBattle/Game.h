#pragma once

// GLEW Include
#include <GL\glew.h>
#include <vector>
#include <stack>
#include "Shader.h"
#include "SceneGraph.h"
#include "RenderingManager.h"
#include "PhysicsManager.h"
#include "SoundManager.h"

class Scene;

class Game
{
	friend class WindowControl;
public:
	Game();

	virtual void Init(class WindowControl* windowControl);

	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();

	int GetWindowHeight();
	int GetWindowWidth();
	
	SceneGraph* sceneGraph;

	RenderingManager* GetRenderManager();
	PhysicsManager* GetPhysicsManager();
	SoundManager* GetSoundManager();
	SceneGraph* GetSceneGraph();

	Scene* GetCurrentScene();

	void PushScene(Scene* newScene);
	void PopScene();
	void ClearScenes();

protected:
	class WindowControl* windowControl;
	virtual void Shutdown();
	virtual ~Game();	
private:

	std::stack<Scene*> m_sceneStack;
};

