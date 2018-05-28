#pragma once

#include "../Maths/types.h"
#include "Enum/KeyCode.h"

class Input
{
public:
	static vec3 mousePosition;

	static bool getKey(KeyCode key);
	static bool getKeyUp(KeyCode key);
	static bool getKeyDown(KeyCode key);
	static bool getMouseButton(int button);
	static bool getMouseButtonUp(int button);
	static bool getMouseButtonDown(int button);
private:
	static bool getKeyUpDown(bool(*getKey)(KeyCode), KeyCode key, bool r);
	static bool getButtonUpDown(bool(*getButton)(int), int button, bool r);
};