#include "Input.h"
#include "Window.h"

#include <iostream>

vec3 Input::mousePosition(0.0f);

bool Input::getKey(KeyCode key)
{
	return (glfwGetKey(Window::current->getWindow(), static_cast<int>(key)));
}

bool Input::getKeyUp(KeyCode key)
{
	return getKeyUpDown(getKey, key, false);
}

bool Input::getKeyDown(KeyCode key)
{

	return getKeyUpDown(getKey, key, true);
}

bool Input::getMouseButton(int button)
{
	return (glfwGetMouseButton(Window::current->getWindow(), button));
}

bool Input::getMouseButtonUp(int button)
{
	return getButtonUpDown(getMouseButton, button, false);
}

bool Input::getMouseButtonDown(int button)
{
	return getButtonUpDown(getMouseButton, button, true);
}

bool Input::getKeyUpDown(bool(*getKey)(KeyCode), KeyCode key, bool r)
{
	static int keyUpDown{ 0 };
	if (getKey(key) && keyUpDown == 0)
	{
		keyUpDown = static_cast<int>(key);
		return r;
	}
	if (!getKey(key) && keyUpDown - static_cast<int>(key) == 0)
	{
		keyUpDown = 0;
		return !r;
	}
	return false;
}

bool Input::getButtonUpDown(bool(*function)(int), int button, bool r)
{
	static int buttonUpDown{ 0 };
	if (function(button) && buttonUpDown == 0)
	{
		buttonUpDown = (button + 1);
		return r;
	}
	if (!function(button) && buttonUpDown - (button + 1) == 0)
	{
		buttonUpDown = 0;
		return !r;
	}
	return false;
}