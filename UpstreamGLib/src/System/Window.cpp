#include "Window.h"
#include <GLFW/glfw3.h>

Window::Window(const char* title, WindowSettings settings)
	:m_GLFWHandle(nullptr)
{
	glfwWindowHint(GLFW_RED_BITS, settings.RedBits);
	glfwWindowHint(GLFW_GREEN_BITS, settings.GreenBits);
	glfwWindowHint(GLFW_BLUE_BITS, settings.BlueBits);
	glfwWindowHint(GLFW_ALPHA_BITS, settings.AplhaBits);
	glfwWindowHint(GLFW_DEPTH_BITS, settings.DepthBits);
	glfwWindowHint(GLFW_STENCIL_BITS, settings.StencilBits);
	glfwWindowHint(GLFW_RESIZABLE, settings.Resizable);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	m_GLFWHandle = glfwCreateWindow(settings.Width, settings.Height, title, NULL, NULL);
	assert(m_GLFWHandle);
	glfwSetWindowUserPointer(m_GLFWHandle, this);
	glfwSetKeyCallback(m_GLFWHandle, Window::__KeyCallback);	
	glfwSetMouseButtonCallback(m_GLFWHandle, Window::__MouseButtonCallback);
	glfwSetWindowCloseCallback(m_GLFWHandle, Window::__WindowCloseCallback);
	glfwSetWindowSizeCallback(m_GLFWHandle, Window::__WindowSizeCallback);
	glfwSetCursorPosCallback(m_GLFWHandle, Window::__MousePosCallback);
}

Window::~Window()
{
	glfwDestroyWindow(m_GLFWHandle);
}

void Window::Update()
{
	glfwSwapBuffers(m_GLFWHandle);
	glfwPollEvents();
}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose(m_GLFWHandle);
}

void Window::MakeCurrent()
{
	glfwMakeContextCurrent(m_GLFWHandle);
}


void Window::__KeyCallback(GLFWwindow* glwin, int key, int, int action, int)
{
	if (action == GLFW_REPEAT) return;
	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glwin));
	assert(window);
	InputManager::GetInstance()->OnKeyboardKey().Publish(InputKeyboardEvent{ (KeyboardKey)key, action == GLFW_PRESS ? true : false });
}

void Window::__WindowCloseCallback(GLFWwindow* glwin)
{
	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glwin));
	assert(window);
	window->m_OnWindowClose.Publish(WindowCloseEvent());
}

void Window::__WindowSizeCallback(GLFWwindow* glwin, int width, int height)
{
	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glwin));
	assert(window);
	window->m_OnWindowResize.Publish(WindowResizeEvent(width, height));
}

void Window::__MouseButtonCallback(GLFWwindow* glwin, int button, int action, int )
{
	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glwin));
	assert(window);
	InputManager::GetInstance()->OnMouseButton().Publish(InputMouseButtonEvent{ (MouseButton)button, action == GLFW_PRESS ? true : false });
}

void Window::__MousePosCallback(GLFWwindow* glwin, double x, double y)
{
	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glwin));
	assert(window);
	InputManager::GetInstance()->OnMouseMove().Publish(InputMouseMoveEvent{ (float)x - window->m_LastMouseX, (float)y - window->m_LastMouseY });
	window->m_LastMouseX = x;
	window->m_LastMouseY = y;
}