#pragma once

// GLEW Include
#include <GL\glew.h>
#include "Shader.h"
#include "Texture.h"
#include "VertexBufferObject.h"
#include "Skybox.h"
#include "Camera.h"

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

private:
	class WindowControl* windowControl;

	VertexBufferObject vboGameObject;

	GLuint VAO, VBO;

	void Shutdown();
	~Game();

	Shader vShader[2], fShader[2];
	ShaderProgram* shaderProgram[2];
	Skybox skybox;

	Texture test;

	Camera camera;

};

