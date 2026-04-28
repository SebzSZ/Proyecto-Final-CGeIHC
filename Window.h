#pragma once
#include <glew.h>
#include <glfw3.h>
#include <stdio.h>

class Window
{
public:
	Window(const char* title);

	int Initialize();

	// Getters del framebuffer
	GLfloat getBufferWith() const { return (GLfloat)bufferWidth; }
	GLfloat getBufferHeight() const { return (GLfloat)bufferHeight; }

	// Métodos para el manejo de la ventana
	bool shouldClose() const { return glfwWindowShouldClose(mainWindow); }
	void swapBuffers() { glfwSwapBuffers(mainWindow); }

	GLFWwindow* getWindow() const { return mainWindow; }

	~Window();

private:
	GLFWwindow* mainWindow = nullptr;
	const char* title;
	GLint width, height;
	GLint bufferWidth = 0;
	GLint bufferHeight = 0;

	// Callbacks
	static void cbKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cbMousePos(GLFWwindow* window, double xPos, double yPos);

	void registerCallbacks();
};