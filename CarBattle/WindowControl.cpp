#include "WindowControl.h"
#include "Game.h"

#include "InputManager.h"

#include "Time.h"

WindowControl::WindowControl()
{
}

bool WindowControl::InitWindow(const char* windowName, int width, int height, int versionMajor, int versionMinor, bool resizableWindow, Game* game)
{
	InitGLFW(versionMajor, versionMinor, resizableWindow);
	
	m_screenWidth = width;
	m_screenHeight = height;

	m_window = glfwCreateWindow(m_screenWidth, m_screenHeight, windowName, nullptr, nullptr);

	if (m_window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	// Make the window our main context on current thread
	glfwMakeContextCurrent(m_window);

	glfwSetKeyCallback(m_window, KeyCallback);
	glfwSetCursorPosCallback(m_window, MouseCallback);
	glfwSetScrollCallback(m_window, ScrollCallback);
	glfwSetMouseButtonCallback(m_window, MouseButtonCallback);
	
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetInputMode(m_window, GLFW_STICKY_KEYS, 0);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return false;
	}

	ResizeWindow(m_screenWidth, m_screenHeight);

	this->game = game;
	game->Init(this);

	Loop();

	game->Shutdown();
	ShutdownWindow();
	return true;	
}

void WindowControl::InitGLFW(int versionMajor, int versionMinor, bool resizableWindow)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, resizableWindow);
}


void WindowControl::ResizeWindow(int width, int height)
{
	m_screenWidth = width;
	m_screenHeight = height;
	glfwSetWindowSize(m_window, m_screenWidth, m_screenHeight);
	glViewport(0, 0, m_screenWidth, m_screenHeight);

/*
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
*/
}

void WindowControl::Loop()
{
	while (!glfwWindowShouldClose(m_window))
	{
		Time::UpdateTime(glfwGetTime());
		// Check call events
		glfwPollEvents();

		Update();

		Render();
	}
	
}

void WindowControl::Update()
{
	game->Update();
}

void WindowControl::Render()
{
	// Rendering and etc
	glClearColor(0.30f, 0.40f, 0.50f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	game->Render();

	glfwSwapBuffers(m_window);
}

void WindowControl::ShutdownWindow()
{
	
	glfwTerminate();
}

void WindowControl::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true,
	// closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (action == GLFW_PRESS)
	{
		InputManager::Instance().keys[key].Down = true;
		InputManager::Instance().keys[key].Press = true;
		InputManager::Instance().keys[key].Release = false;
	}
	else if (action == GLFW_RELEASE)
	{
		InputManager::Instance().keys[key].Down = false;
		InputManager::Instance().keys[key].Press = false;
		InputManager::Instance().keys[key].Release = true;
	}

}

void WindowControl::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{

	InputManager::Instance().MouseX = xpos;
	InputManager::Instance().MouseY = ypos;

}

void WindowControl::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		InputManager::Instance().MouseClick[button].Down = true;
		InputManager::Instance().MouseClick[button].Press = true;
		InputManager::Instance().MouseClick[button].Release = false;
	}
	else if (action == GLFW_RELEASE)
	{
		InputManager::Instance().MouseClick[button].Down = false;
		InputManager::Instance().MouseClick[button].Press = false;
		InputManager::Instance().MouseClick[button].Release = true;
	}
}

void WindowControl::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{

}

GLFWwindow* WindowControl::GetWindow()
{
	return m_window;
}

int WindowControl::GetHeight()
{
	return m_screenHeight;
}

int WindowControl::GetWidth()
{
	return m_screenWidth;
}

WindowControl::~WindowControl()
{
	delete game;
}
