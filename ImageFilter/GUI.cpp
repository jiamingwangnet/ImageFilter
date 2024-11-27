#include "include/GUI.h"
#include "include/ShaderManager.h"
#include "include/ImageManager.h"
#include "include/Window.h"
#include <fstream>
#include <sstream>
#include <iostream>

void GUI::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	io = &ImGui::GetIO();
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	// disable ini file
	io->IniFilename = nullptr;

	ImGui_ImplGlfw_InitForOpenGL(window.GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 460");

	ImGui::PushStyleColor(ImGuiCol_Header, 0xff151515);
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, 0xff171717);
	ImGui::PushStyleColor(ImGuiCol_HeaderActive, 0xff1f1f1f);

	ImGui::PushStyleColor(ImGuiCol_FrameBg, 0xff151515);
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, 0xff171717);
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive, 0xff1f1f1f);

	ImGui::PushStyleColor(ImGuiCol_Button, 0xff151515);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0xff171717);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, 0xff1f1f1f);

	ImGui::PushStyleColor(ImGuiCol_ResizeGrip, 0xff151515);
	ImGui::PushStyleColor(ImGuiCol_ResizeGripHovered, 0xff171717);
	ImGui::PushStyleColor(ImGuiCol_ResizeGripActive, 0xff1f1f1f);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, 0xff060606);
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, 0xff0f0f0f);
	ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, 0xff010101);
}

void GUI::RenderStart()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void GUI::CreateGui()
{
	static bool firstRender = true;

	if (firstRender)
	{
		ImGui::SetNextWindowPos({ 10.0, 10.0 });
		ImGui::SetNextWindowSize({ 400.0, 600.0 });
		firstRender = false;
	}

	ImGui::Begin("File");

	if (ImGui::CollapsingHeader("Image File", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Use Camera");
		ImGui::SameLine();
		ImGui::Checkbox("##UseCamera", &useCamera);
		if (!useCamera)
		{
			window.QueueFunc(
				[this](Renderer& renderer, ImageManager& imageManager)
				{
					imageManager.CloseCamera();
				}
			);

			ImGui::Text("Image File");
			ImGui::SameLine();
			ImGui::InputText("##ImageFile", &data.imagePath);

			if (ImGui::Button("Open"))
			{
				window.QueueFunc(
					[this](Renderer& renderer, ImageManager& imageManager)
					{
						imageManager.Load(data.imagePath);
					}
				);
			}
		}
		else
		{
			ImGui::Text("2x Upscale");
			ImGui::SameLine();
			ImGui::Checkbox("##2xUpscale", &upscale);

			ImGui::Text("Device");
			ImGui::SameLine();
			if (ImGui::InputInt("##Device", &device, 1, 1))
			{
				window.QueueFunc(
					[this](Renderer& renderer, ImageManager& imageManager)
					{
						imageManager.CloseCamera();
					}
				);
			}

			window.QueueFunc(
				[this](Renderer& renderer, ImageManager& imageManager)
				{
					imageManager.UpscaleImage(upscale);
					imageManager.CamCapture(device);
				}
			);
		}

		ImGui::Text("Save Path");
		ImGui::SameLine();
		ImGui::InputText("##SavePath", &data.outputPath);
		if (ImGui::Button("Save"))
		{
			window.QueueFunc(
				[this](Renderer& renderer, ImageManager& imageManager)
				{
					imageManager.ApplyEffect(activeShader);
					imageManager.SaveOutput(data.outputPath);
				}
			);
		}


		ImGui::NewLine();
	}

	if (ImGui::CollapsingHeader("Compute Shader File", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Shader Path");
		ImGui::SameLine();
		ImGui::InputText("##ShaderPath", &data.shaderPath);

		if (ImGui::Button("Add") && data.shaderPath != "")
		{
			std::ifstream file{ data.shaderPath };
			if (file)
			{
				std::stringstream sourceStream;
				sourceStream << file.rdbuf();

				ShaderData newShader;
				newShader.name = ComputeShader::MetaGetName(sourceStream.str());
				newShader.path = data.shaderPath;

				ShaderManager::CreateShader(data.shaderPath, newShader.name, ShaderManager::SHADER_TYPE::COMPUTE);

				shaders.push_back(newShader);
			}
			else
			{
				shaders.push_back(ShaderData{ .path = data.shaderPath, .found = false});
			}
		}

		ImGui::NewLine();
	}

	ImGui::End();

	int id = 0;
	for (auto sIt = shaders.begin(); sIt != shaders.end();)
	{
		ShaderData& shader = *sIt;
		ImGui::PushID(id);

		if (!shader.found)
		{
			ImGui::Begin(shader.path.c_str(), &shader.winOpen);
			
			ImGui::Text("Shader Not Found.");

			ImGui::End();

			ImGui::PopID();
			id++;

			if (!shader.winOpen)
				sIt = shaders.erase(sIt);
			else
				++sIt;

			continue;
		}

		ImGui::Begin(shader.name.c_str());

		ImGui::Text(shader.path.c_str());

		ImGui::NewLine();

		ComputeShader& compute = ShaderManager::GetShader<ComputeShader>(shader.name);

		for (const std::pair<std::string, ComputeShader::ValueTypes>& uniform : compute.GetUniforms())
		{
			if (uniform.second != ComputeShader::ValueTypes::Other)
			{
				switch (uniform.second)
				{
				case ComputeShader::ValueTypes::Int:
				{
					auto uvalue = shader.uniforms.find(uniform.first);
					if (uvalue == shader.uniforms.end())
					{
						uvalue = shader.uniforms.insert({ uniform.first, UniformValue{0} }).first;
						uvalue->second.intv = ComputeShader::MetaGetUniformInt(compute.GetSource(), uniform.first);
					}

					ImGui::InputInt(uniform.first.c_str(), &uvalue->second.intv);
					compute.SetInt(uniform.first, uvalue->second.intv);
					break;
				}
				case ComputeShader::ValueTypes::Float:
				{
					auto uvalue = shader.uniforms.find(uniform.first);
					if (uvalue == shader.uniforms.end())
					{
						uvalue = shader.uniforms.insert({ uniform.first, UniformValue{0} }).first;
						uvalue->second.floatv = ComputeShader::MetaGetUniformFloat(compute.GetSource(), uniform.first);
					}

					ImGui::InputFloat(uniform.first.c_str(), &uvalue->second.floatv);
					compute.SetFloat(uniform.first, uvalue->second.floatv);
					break;
				}
				}
			}
		}

		ImGui::NewLine();

		bool del = ImGui::Button("Delete");
		
		if (ImGui::Button("Run"))
		{
			if (!useCamera)
			{
				window.QueueFunc(
					[&shader](Renderer& renderer, ImageManager& imageManager)
					{
						imageManager.ApplyEffect(shader.name);
					}
				);
			}

			activeShader = shader.name;
		}

		if (ImGui::Button("Recompile"))
		{
			window.QueueFunc(
				[&shader](Renderer& renderer, ImageManager& imageManager)
				{
					ShaderManager::GetShader<ComputeShader>(shader.name).Build();
				}
			);
		}

		ImGui::End();

		ImGui::PopID();
		id++;

		if (del)
		{
			sIt = shaders.erase(sIt);
			if (activeShader == shader.name) activeShader = "";
		}
		else
			++sIt;
	}

	if (useCamera && activeShader != "")
	{
		window.QueueFunc(
			[this](Renderer& renderer, ImageManager& imageManager)
			{
				imageManager.ApplyEffect(activeShader);
			}
		);
	}
}

void GUI::RenderEnd()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
