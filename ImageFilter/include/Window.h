#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "ImageManager.h"
#include "GUI.h"
#include <queue>
#include <functional>

class Window
{
public:
	// functions that needed to be called during the GUI stage that is called later
	using queuerenderfunc = std::function<void(Renderer&, ImageManager&)>;
public:
	Window(int width, int height);
	void Create();

	void MainLoop(); // application loop for input polling

	GLFWwindow* GetWindow() const { return window; };

	int GetWidth() const 
	{ 
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		return width;
	}
	int GetHeight() const 
	{ 
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		return height;
	}

	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

	void QueueFunc(queuerenderfunc func)
	{
		renderFuncs.push(func);
	}

	const GUI& GetGUI() const { return gui; }

private:
	int initWidth;
	int initHeight;

	GLFWwindow* window = nullptr;

	Renderer renderer;
	ImageManager imageManager;
	GUI gui;

	std::queue<queuerenderfunc> renderFuncs;
};