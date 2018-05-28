#include "Core.h"

Core::Core()
	: window(800, 600, "Test")
{}

Core::Core(int width, int height, const char* name)
	: window(width, height, name)
{}

void Core::Run()
{
	Start();

	float lastFrame{ 0.0f };

	while (!window.close())
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		Time::deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		window.clearBuffers();

		Update();

		window.updateWindow();
	}

	End();
}