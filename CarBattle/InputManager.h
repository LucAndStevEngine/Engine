#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#pragma once
#include "Singleton.h"

#define KEYPRESSES 1024
#define MOUSEBUTTONS 8

struct ButtonState
{
	bool Press;
	bool Down;
	bool Release;
};

class InputManager : public Singleton<InputManager>
{
	friend class Singleton<InputManager>;

public:
	// Key press booleans
	ButtonState keys[KEYPRESSES];

	// MouseClick booleans
	ButtonState MouseClick[MOUSEBUTTONS];

	// Mouse position X and Y
	double MouseX, MouseY;

	bool KeyPress(int key);
	bool KeyDown(int key);
	bool KeyUp(int key);


	bool MousePress(int key);
	bool MouseDown(int key);
	bool MouseUp(int key);

	void EndFrame();

	~InputManager();

private:
	InputManager();
};

#endif