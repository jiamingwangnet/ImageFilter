#include "include/InputManager.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include "include/Window.h"

InputManager::scrollfunc InputManager::scrollcallback = [](double,double) {};
InputManager::mousebtnfunc InputManager::mousebtncallback = [](int, int, int) {};
InputManager::cursorfunc InputManager::cursorcallback = [](double,double) {};
double InputManager::cursorX = 0.0;
double InputManager::cursorY = 0.0;
double InputManager::lastCurX = 0.0;
double InputManager::lastCurY = 0.0;

void InputManager::BindCallbacks(GLFWwindow* window)
{
	glfwSetScrollCallback(window, &InputManager::ScrollCallback);
	glfwSetMouseButtonCallback(window, &InputManager::MouseButtonCallback);
	glfwSetCursorPosCallback(window, &InputManager::CursorCallback);
}

void InputManager::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	scrollcallback(xoffset, yoffset);
}

void InputManager::CursorCallback(GLFWwindow* window, double xpos, double ypos)
{
	lastCurX = cursorX;
	lastCurY = cursorY;

	cursorX = xpos;
	cursorY = ypos;

	cursorcallback(cursorX, cursorY);
}

void InputManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	mousebtncallback(button, action, mods);
}
