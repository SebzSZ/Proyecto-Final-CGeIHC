#include "InputManager.h"
#include "Window.h"

Window::Window(const char* title)
	: title(title)
{
	if (!glfwInit())
	{
		printf("[Window] Error: No se pudo inicializar GLFW.\n");
		return;
	}

	// Detectar las dimensiones de la pantalla primaria
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	width = mode->width;
	height = mode->height;

	glfwTerminate();
}

int Window::Initialize()
{
	if (!glfwInit())
	{
		printf("[Window] Error: No se pudo inicializar GLFW.\n");
		return -1;
	}

	// OpenGL 3.3 Core
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Crear ventana en pantalla completa
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	mainWindow = glfwCreateWindow(width, height, title, monitor, nullptr);
	if (!mainWindow)
	{
		printf("[Window] Error: No se pudo crear la ventana.\n");
		glfwTerminate();
		return -1;
	}

	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
	glfwMakeContextCurrent(mainWindow);

	// Ocultar el cursor
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetWindowUserPointer(mainWindow, this);
	registerCallbacks();

	// Inicializar GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		printf("[Window] Error: No se pudo inicializar GLEW.\n");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, bufferWidth, bufferHeight);

	return 0;
}

void Window::registerCallbacks()
{
	glfwSetKeyCallback(mainWindow, cbKeyboard);
	glfwSetCursorPosCallback(mainWindow, cbMousePos);
}

// Callbacks
void Window::cbKeyboard(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	InputManager::getInstance().onKeyEvent(key, action);
}

void Window::cbMousePos(GLFWwindow* /*window*/, double xPos, double yPos)
{
	InputManager::getInstance().onMouseMove(xPos, yPos);
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}