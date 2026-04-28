#include <glew.h>
#include <glfw3.h>
#include "InputManager.h"
#include "Window.h"

int main()
{
	Window mainWindow("Proyecto Final - CGeIHC");
	if (mainWindow.Initialize() != 0) return -1;

	while (!mainWindow.shouldClose())
	{
		glfwPollEvents();
		InputManager::getInstance().beginFrame();

		glClearColor(0.0f, 0.4f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mainWindow.swapBuffers();
	}
}