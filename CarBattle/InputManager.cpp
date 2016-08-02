#include "InputManager.h"

InputManager::InputManager() : MouseX(0), MouseY(0)
{
	for (int i = 0; i < MOUSEBUTTONS; i++)
	{
		MouseClick[i].Down = false;
		MouseClick[i].Press = false;
		MouseClick[i].Release = false;
	}

	for (int i = 0; i < KEYPRESSES; i++)
	{
		keys[i].Down = false;
		keys[i].Press = false;
		keys[i].Release = false;
	}
}

void InputManager::EndFrame()
{
	for (int i = 0; i < MOUSEBUTTONS; i++)
	{
		MouseClick[i].Press = false;
		MouseClick[i].Release = false;
	}

	for (int i = 0; i < KEYPRESSES; i++)
	{
		keys[i].Press = false;
		keys[i].Release = false;
	}
}

bool InputManager::KeyPress(int key)
{
	return keys[key].Press;
}

bool InputManager::KeyDown(int key)
{
	return keys[key].Down;
}

bool InputManager::KeyUp(int key)
{
	return keys[key].Release;
}

bool InputManager::MousePress(int key)
{
	return MouseClick[key].Press;
}

bool InputManager::MouseDown(int key)
{
	return MouseClick[key].Down;
}

bool InputManager::MouseUp(int key)
{
	return MouseClick[key].Release;
}

InputManager::~InputManager()
{
}
