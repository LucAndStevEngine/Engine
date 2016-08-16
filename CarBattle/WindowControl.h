#include <iostream>

// GLEW Include
#include <GL\glew.h>

// GLFW Include
#include <GLFW\glfw3.h>

// Threading
#include <thread>

#pragma once
class WindowControl
{
public:
	bool InitWindow(const char* windowName, int width, int height, int versionMajor, int versionMinor, bool resizableWindow, class Game* game);

	void ResizeWindow(int width, int height);

	void Loop();

	void Render();
	void Update();
	void FixedUpdate();

	WindowControl();
	~WindowControl();

	GLFWwindow* GetWindow();
	int GetWidth();
	int GetHeight();

private:
	void InitGLFW(int versionMajor, int versionMinor, bool resizableWindow);
	void ShutdownWindow();

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void MaximizeCallBack(GLFWwindow* window, int width, int height);

	GLFWwindow* m_window;

	static int m_screenWidth;
	static int m_screenHeight;

	class Game* game;

	std::thread update;
	bool gameRunning;
};

