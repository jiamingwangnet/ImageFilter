#include "include/ShaderManager.h"
#include <stdexcept>

std::unordered_map<std::string, std::unique_ptr<ComputeShader>> ShaderManager::storage;

void ShaderManager::CreateShader(const std::string& path, const std::string& name)
{
	storage.insert({ name, std::make_unique<ComputeShader>(path) });
}

ComputeShader& ShaderManager::GetShader(const std::string& name)
{
	try
	{
		ComputeShader& shader = *(storage.at(name));

		return shader;
	}
	catch (const std::out_of_range& error)
	{
		throw std::out_of_range{ (std::string)"Can't find shader " + name };
	}
}
