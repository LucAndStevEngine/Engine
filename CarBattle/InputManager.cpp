#include "InputManager.h"
#include "GLFW\glfw3.h"

void InputManager::UpdateInformation(ButtonState & state)
{
	state.Press = false;
	state.Release = false;


	if (state.pressed)
	{
		state.Press = true;
		state.Down = true;
		state.pressed = false;
	}

	if (state.released)
	{
		state.Release = true;
		state.Down = false;
		state.released = false;
	}
}

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
		UpdateInformation(MouseClick[i]);	
	}

	for (int i = 0; i < KEYPRESSES; i++)
	{
		UpdateInformation(keys[i]);
	}

	for (int i = 0; i < JOYSTICKS; i++)
	{
		if (!joySticks[i].bIsPluggedIn)
		{
			continue;
		}

		for (int j = 0; j < joySticks[i].JoystickButtons.size(); j++)
		{
			UpdateInformation(joySticks[i].JoystickButtons[j]);
		}
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

bool InputManager::GetJoyStickButtonPress(int JoystickNumber, int button)
{
	if (joySticks[JoystickNumber].bIsPluggedIn && button < joySticks[JoystickNumber].JoystickButtons.size())
	{
		return joySticks[JoystickNumber].JoystickButtons[button].Press;
	}

	return false;
}

bool InputManager::GetJoyStickButtonDown(int JoystickNumber, int button)
{
	if (joySticks[JoystickNumber].bIsPluggedIn && button < joySticks[JoystickNumber].JoystickButtons.size())
	{
		return joySticks[JoystickNumber].JoystickButtons[button].Down;
	}

	return false;
}

bool InputManager::GetJoyStickButtonUp(int JoystickNumber, int button)
{
	if (joySticks[JoystickNumber].bIsPluggedIn && button < joySticks[JoystickNumber].JoystickButtons.size())
	{
		return joySticks[JoystickNumber].JoystickButtons[button].Release;
	}

	return false;
}



float InputManager::GetJoyStickAxis(int JoystickNumber, int axis)
{

	if (joySticks[JoystickNumber].bIsPluggedIn)
	{
		return -joySticks[JoystickNumber].JoyStickAxis[axis];
	}

	return 0;
}

void InputManager::JoyStickUpdate()
{
	for (int i = 0; i < JOYSTICKS; i++)
	{
		if (glfwJoystickPresent(i))
		{
			int axisCount;
			float* axes;
			axes = (float*)glfwGetJoystickAxes(i, &axisCount);

			int buttonCount;
			unsigned char* buttons;
			buttons = (unsigned char*)glfwGetJoystickButtons(i, &buttonCount);

			joySticks[i].JoyStickAxis = axes;
			joySticks[i].bIsPluggedIn = true;

			for (int j = 0; j < buttonCount; j++)
			{
				if (j < joySticks[i].JoystickButtons.size())
				{
					if (buttons[j] == GLFW_PRESS && !joySticks[i].JoystickButtons[j].Down)
					{
						joySticks[i].JoystickButtons[j].pressed = true;
					}

					if (buttons[j] == GLFW_RELEASE && joySticks[i].JoystickButtons[j].Down)
					{
						joySticks[i].JoystickButtons[j].released = false;
					}
				}
				else
				{
					ButtonState newButton;
					newButton.Press = false;
					newButton.Down = false;
					newButton.Release = false;
					joySticks[i].JoystickButtons.push_back(newButton);
					j--;
				}
			}
		} 
		else
		{
			joySticks[i].bIsPluggedIn = false;
		}
	}
}

InputManager::~InputManager()
{
}
