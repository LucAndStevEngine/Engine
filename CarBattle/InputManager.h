#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#pragma once
#include "Singleton.h"
#include <vector>
#define JOYSTICKS 4

#define KEYPRESSES 1024
#define MOUSEBUTTONS 8

struct ButtonState
{
	bool pressed = false;
	bool released = false;

	bool Press;
	bool Down;
	bool Release;
};

struct JoyStickState
{
	std::vector<ButtonState> JoystickButtons;
	float* JoyStickAxis;
	bool bIsPluggedIn;
};

class InputManager : public Singleton<InputManager>
{
	friend class Singleton<InputManager>;

public:
	// Key press booleans
	ButtonState keys[KEYPRESSES];

	// MouseClick booleans
	ButtonState MouseClick[MOUSEBUTTONS];

	//All the info for each joystick
	JoyStickState joySticks[JOYSTICKS];

	// Mouse position X and Y
	double MouseX, MouseY;

	bool KeyPress(int key);
	bool KeyDown(int key);
	bool KeyUp(int key);

	bool MousePress(int key);
	bool MouseDown(int key);
	bool MouseUp(int key);

	bool GetJoyStickButtonPress(int JoystickNumber, int button);
	bool GetJoyStickButtonDown(int JoystickNumber, int button);
	bool GetJoyStickButtonUp(int JoystickNumber, int button);

	float GetJoyStickAxis(int JoystickNumber, int axis);
	
	void JoyStickUpdate();

	void EndFrame();

	~InputManager();

private:
	void UpdateInformation(ButtonState &state);
	InputManager();
};

#endif