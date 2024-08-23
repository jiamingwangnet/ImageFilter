#include "include/GUI.h"
#include "include/ShaderManager.h"
#include "include/ImageManager.h"
#include "include/Window.h"
#include <fstream>
#include <sstream>

void GUI::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	io = &ImGui::GetIO();
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui_ImplGlfw_InitForOpenGL(window.GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

void GUI::RenderStart()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void GUI::CreateGui()
{
	ImGui::Begin("File");

	if (ImGui::CollapsingHeader("Image File"))
	{
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

		ImGui::NewLine();
	}

	if (ImGui::CollapsingHeader("Shader File"))
	{
		ImGui::Text("Shader Path");
		ImGui::SameLine();
		ImGui::InputText("##ShaderPath", &data.shaderPath);

		if (ImGui::Button("Add") && data.shaderPath != "")
		{
			std::ifstream file{ data.shaderPath };
			std::stringstream sourceStream;
			sourceStream << file.rdbuf();

			ShaderData newShader;
			newShader.name = ComputeShader::MetaGetName(sourceStream.str());
			newShader.path = data.shaderPath;

			ShaderManager::CreateShader(data.shaderPath, newShader.name);

			shaders.push_back(newShader);
		}

		ImGui::NewLine();
	}

	ImGui::End();

	int id = 0;
	for (auto sIt = shaders.begin(); sIt != shaders.end();)
	{
		ShaderData& shader = *sIt;
		ImGui::PushID(id);

		ImGui::Begin(shader.name.c_str());

		ImGui::Text(shader.path.c_str());

		ImGui::NewLine();

		ComputeShader& compute = ShaderManager::GetShader(shader.name);

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
			window.QueueFunc(
				[&shader](Renderer& renderer, ImageManager& imageManager)
				{
					imageManager.ApplyEffect(shader.name);
				}
			);
		}

		if (ImGui::Button("Recompile"))
		{
			window.QueueFunc(
				[&shader](Renderer& renderer, ImageManager& imageManager)
				{
					ShaderManager::GetShader(shader.name).Build();
				}
			);
		}

		ImGui::End();

		ImGui::PopID();
		id++;

		if (del)
			sIt = shaders.erase(sIt);
		else
			++sIt;
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
