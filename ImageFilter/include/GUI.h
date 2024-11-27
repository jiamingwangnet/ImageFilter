#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_stdlib.h"
#include <unordered_map>
#include "ShaderBase.h"

class Window;

class GUI
{
public:
	struct InputData
	{
		std::string imagePath = "";
		std::string shaderPath = "";
		std::string shaderName = "";
		std::string outputPath = "";
	};
	struct ShaderData
	{
		std::string name;
		std::string path;

		std::unordered_map<std::string, UniformValue> uniforms;

		bool found = true;
		bool winOpen = true;
	};
public:
	GUI(Window& window)
		: window{window}
	{}

	void Init();
	void RenderStart();
	void CreateGui();
	void RenderEnd();
	void Shutdown();

	ImGuiIO* GetIO() const { return io; }

private:
	ImGuiIO* io = nullptr;
	InputData data;
	std::vector<ShaderData> shaders;
	Window& window;
	bool useCamera = false;
	bool upscale = true;
	std::string activeShader = "";
};