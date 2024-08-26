#include "include/Window.h"
#include <stdexcept>
#include <glad/gl.h>
#include <iostream>
#include "include/ShaderManager.h"
#include "include/InputManager.h"

Window::Window(int width, int height)
	: initWidth{width}, initHeight{height}, gui{*this}
{

}

void Window::Create()
{
	if (!glfwInit())
	{
		std::cerr << "GLFW Init failed." << std::endl;
		throw std::runtime_error{ "GLFW Init failed." };
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(initWidth, initHeight, "Image Filter", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cerr << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
		throw std::runtime_error{ "Failed to create GLFW window." };
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize OpenGL context." << std::endl;
		throw std::runtime_error{ "Failed to initialize OpenGL context." };
	}

	glViewport(0, 0, initWidth, initHeight);

	glfwSetFramebufferSizeCallback(window, &Window::FramebufferSizeCallback);
	InputManager::BindCallbacks(window);

	gui.Init();
	renderer.Init(this);
}

void Window::MainLoop()
{
	// query limitations
	// -----------------
	int max_compute_work_group_count[3] = { 0 };
	int max_compute_work_group_size[3] = { 0 };
	int max_compute_work_group_invocations;

	for (int idx = 0; idx < 3; idx++)
	{
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, idx, &max_compute_work_group_count[idx]);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, idx, &max_compute_work_group_size[idx]);
	}
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &max_compute_work_group_invocations);

	std::cout << "OpenGL Limitations: " << std::endl;
	std::cout << "maximum number of work groups in X dimension " << max_compute_work_group_count[0] << std::endl;
	std::cout << "maximum number of work groups in Y dimension " << max_compute_work_group_count[1] << std::endl;
	std::cout << "maximum number of work groups in Z dimension " << max_compute_work_group_count[2] << std::endl;

	std::cout << "maximum size of a work group in X dimension " << max_compute_work_group_size[0] << std::endl;
	std::cout << "maximum size of a work group in Y dimension " << max_compute_work_group_size[1] << std::endl;
	std::cout << "maximum size of a work group in Z dimension " << max_compute_work_group_size[2] << std::endl;

	std::cout << "Number of invocations in a single local work group that may be dispatched to a compute shader " << max_compute_work_group_invocations << std::endl;

	glfwSwapInterval(1);

	imageManager.BeginCamThread();

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		gui.RenderStart();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		while (renderFuncs.size() > 0)
		{
			renderFuncs.front()(renderer, imageManager);
			renderFuncs.pop();
		}
		renderer.RenderImage(*this, imageManager.GetEditedImage());

		gui.CreateGui();

		gui.RenderEnd();
		glfwSwapBuffers(window); // TODO: only swap buffers when app state was changed
	}

	imageManager.EndCamThread();
}

void Window::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
