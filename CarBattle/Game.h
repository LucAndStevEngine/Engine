#pragma once

// GLEW Include
#include <GL\glew.h>
#include <vector>
#include "Shader.h"
#include "SceneGraph.h"

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

protected:
	std::vector<ShaderProgram*> programs;
	std::vector<Shader*> shaders;

	class WindowControl* windowControl;
	SceneGraph* sceneGraph;
	virtual void Shutdown();
	virtual ~Game();
private:
	class Skybox* skybox;
	class Camera* camera;
};

