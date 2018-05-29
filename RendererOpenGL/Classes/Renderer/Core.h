#pragma once

#include <glad\glad.h>
#include "Window.h"
#include "Time.h"

class Core
{
private:
	Window window;

public:
	Core();
	Core(int width, int height, const char* name);

	void Run();
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void End() = 0;
};

#define CORE_MAIN(a)								\
int main()									\
{										\
	a *app = new a;								\
	app->Run();								\
	delete app;								\
	return 0;								\
}													
