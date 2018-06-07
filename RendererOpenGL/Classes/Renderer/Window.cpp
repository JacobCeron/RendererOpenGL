#include <glad\glad.h>
#include <cassert>
#include "Window.h"
#include "Input.h"

Window* Window::current{ nullptr };

Window::Window(int width, int height, const char* name)
	: width{ width }, height{ height }, name{ name }
{
	current = this;
	init();
}

void Window::init()
{
	glfwInit();
	window = glfwCreateWindow(getWidth(), getHeight(), name, nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) && "Couldn't initilaize OpenGL");

	glEnable(GL_DEPTH_TEST);
}

void Window::setColor(const vec4& color)
{
	this->color = color;
}

void Window::clearBuffers()
{
	glViewport(0, 0, getWidth(), getHeight());
	glClearBufferfv(GL_COLOR, 0, &color[0]);
	glClearBufferfi(GL_DEPTH_STENCIL, 0, 1.0f, 0);
}

void Window::updateWindow()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

int Window::getWidth()
{
	return width;
}

int Window::getHeight()
{
	return height;
}

bool Window::close()
{
	return glfwWindowShouldClose(window);
}

void Window::shouldClose(bool value)
{
	glfwSetWindowShouldClose(window, value);
}

GLFWwindow* Window::getWindow()
{
	return window;
}

void Window::framebufferSize(int width, int height)
{
	this->width = width;
	this->height = height;
}

void Window::cursorPos(double xPos, double yPos)
{
	Input::mousePosition.x = static_cast<float>(xPos);
	Input::mousePosition.y = static_cast<float>(getHeight() - yPos);
	Input::mousePosition.z = 0.0f;
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	current->framebufferSize(width, height);
}

void Window::cursorPosCallback(GLFWwindow* window, double xPos, double yPos)
{
	current->cursorPos(xPos, yPos);
}

Window::~Window()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}