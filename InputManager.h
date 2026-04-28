#pragma once
#include <glew.h>
#include <glfw3.h>

class InputManager
{
public:
	static InputManager& getInstance()
	{
		static InputManager instance;
		return instance;
	}

	// Teclado
	bool isKeyDown(int glfwKey) const { return keys[glfwKey]; }
	bool isKeyPressed(int glfwKey) const { return keys[glfwKey] && !prevKeys[glfwKey]; }
	bool isKeyReleased(int glfwKey) const { return !keys[glfwKey] && prevKeys[glfwKey]; }

	// Mouse
	float getMouseDeltaX() const { return mouseDeltaX; }
	float getMouseDeltaY() const { return mouseDeltaY; }

	// Capturar valores del mouse al inicio de cada frame
	void beginFrame()
	{
		for (int i = 0; i < 1024; ++i) prevKeys[i] = keys[i];
		mouseDeltaX = rawMouseX - lastMouseX;
		mouseDeltaY = rawMouseY - lastMouseY;
		lastMouseX = rawMouseX;
		lastMouseY = rawMouseY;
	}

	void onKeyEvent(int key, int action)
	{
		if (key >= 0 && key < 1024)
		{
			if (action == GLFW_PRESS) keys[key] = true;
			if (action == GLFW_RELEASE) keys[key] = false;
		}
	}

	void onMouseMove(double xPos, double yPos)
	{
		if (firstMouse)
		{
			lastMouseX = (float)xPos;
			lastMouseY = (float)yPos;
			firstMouse = false;
		}
		rawMouseX = (float)xPos;
		rawMouseY = (float)yPos;
	}

private:
	InputManager() = default;
	InputManager(const InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;

	bool keys[1024] = {};
	bool prevKeys[1024] = {};
	bool mouseButtons[8] = {};

	float rawMouseX = 0.0f, rawMouseY = 0.0f;
	float lastMouseX = 0.0f, lastMouseY = 0.0f;
	float mouseDeltaX = 0.0f, mouseDeltaY = 0.0f;
	bool firstMouse = true;
};