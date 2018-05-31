#pragma once

#include <GLFW\glfw3.h>
#include "../maths/maths.h"

class Window
{
public:
	static Window* current;
private:
	int width;
	int height;
	const char* name;
	vec4 color;
	GLFWwindow* window;

public:
	Window(int width, int height, const char* name);
	void init();
	void setColor(const vec4& color);
	void clearBuffers();
	void updateWindow();
	int getWidth();
	int getHeight();
	bool close();
	~Window();
private:
	GLFWwindow* getWindow();
	void framebufferSize(int width, int height);
	void cursorPos(double xPos, double yPos);
	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
	static void cursorPosCallback(GLFWwindow* window, double xPos, double yPos);

	friend class Input;
};